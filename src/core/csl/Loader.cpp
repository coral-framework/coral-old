/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Loader.h"
#include "scanner.hh"
#include "parser.hh"
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodBuilder.h>
#include <co/IInterface.h>
#include <co/IException.h>
#include <co/TypeLoadException.h>
#include <co/ITypeTransaction.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/OS.h>

#ifdef PUSH_ERROR
#undef PUSH_ERROR
#endif

#define PUSH_ERROR( loc, msg ) { std::stringstream errorSS; \
	errorSS << msg; pushError( loc, errorSS.str() ); }

#define CATCH_ERRORS( loc, code ) try { code; } \
	catch( co::Exception& e ) { pushError( loc, e.getMessage() ); return; }

namespace co {
namespace csl {

Loader::Loader()
{
	// empty
}

Loader::~Loader()
{
	// empty
}

bool Loader::parse( const std::string& filename )
{
	_filename = &filename;
	
	// extract the CSL file's base name
	size_t startPos = filename.rfind( CORAL_OS_DIR_SEP );
	startPos = ( startPos == std::string::npos ? 0 : startPos + 1 );
	size_t lastDotPos = filename.rfind( '.' );
	_cslFileBaseName.assign( filename, startPos, lastDotPos - startPos );

	// open file
	FILE* file = fopen( filename.c_str(), "rt" );
	if( !file )
	{
		pushError( filename, "could not open file" );
		return false;
	}

	yyscan_t scanner;
	csl_lex_init( &scanner );
	csl_set_in( file, scanner );

	try
	{
		Parser parser( *this, scanner );
		//parser.set_debug_level( 1 );
		if( parser.parse() != 0 )
		{
			assert( getError() );
			if( !getError() )
				pushError( filename, "unexpected parse error" );
		}
	}
	catch( std::exception& e )
	{
		pushError( filename, std::string( "unexpected exception: " ) + e.what() );
	}
	catch( ... )
	{		
		pushError( filename, "unexpected unknown exception" );
	}

	csl_lex_destroy( scanner );
	fclose( file );
	_filename = NULL;

	return !_error.isValid();
}

void Loader::pushError( const location& loc, const std::string& message )
{
	_error = new Error( loc, message, _error.get() );
}

void Loader::pushError( const std::string& filename, const std::string& message )
{
	_error = new Error( filename, message, _error.get() );
}	

void Loader::pushError( const std::string& message )
{
	_error = new Error( message, _error.get() );	
}

void Loader::setError( Error* error )
{
	assert( !_error.isValid() );
	_error = error;	
}

std::string* Loader::newString()
{
	_stringPool.push_back( std::string() );
	return &_stringPool.back();
}

std::string* Loader::newString( const char* cstr )
{
	_stringPool.push_back( cstr );
	return &_stringPool.back();
}

void Loader::onComment( const location&, const std::string& text )
{
	static const char* START_END_BLANKS = " *\t\r\n/";

	bool isRetroactive = false;

	// discard the comment's opening symbol
	size_t start = 0;

	// is this retroactive documentation?
	if( !text.empty() && text[0] == '<' )
	{
		start = 1;
		isRetroactive = true;
	}

	start = text.find_first_not_of( START_END_BLANKS, start );

	// ignore a comment if it starts with a dash (useful for separators)
	if( text.length() > start && text[start] == '-' )
		return;

	// discard "blanks" at the end
	size_t end = text.find_last_not_of( START_END_BLANKS );

	if( start == end )
		return;

	// extract and add the doc text
	size_t len = ( end - start ) + 1;

	std::string docText;
	docText.reserve( len + 1 );
	if( !_docBuffer.empty() )
		docText.push_back( '\n' );
	docText.append( text, start, len );

	if( isRetroactive )
		addDocumentation( _lastMember, docText );
	else
		_docBuffer.append( docText );
}

void Loader::onCppBlock( const location&, const std::string& text )
{
	// removes the opening and closing c++ tags.
	addCppBlock( text );
}

void Loader::onTypeSpec( const location& specLoc, const location& nameLoc,
							const std::string& typeName, TypeKind kind )
{
	if( _typeBuilder.isValid() )
		pushError( specLoc, "only one type specification is allowed per file" );
	else if( typeName != _cslFileBaseName )
		pushError( nameLoc, "the name of the defined type must match its filename" );
	else
	{
		_typeBuilder = createTypeBuilder( typeName, kind );

		handleDocumentation( std::string() );
		resolveImports();
	}
}

void Loader::onTypeDecl( const location& loc, const std::string& name, bool isArray )
{
	_lastDeclTypeIsArray = isArray;
	_lastDeclTypeName = name;
	_lastDeclTypeLocation = loc;
}

void Loader::onImport( const location& loc, const std::string& importTypeName )
{
	/*
		Notice: the imported type cannot be loaded at this point because of
		potential cyclic dependencies.
	 */

	if( _typeBuilder.isValid() )
	{
		pushError( loc, "all import clauses must come before the type specification" );
		return;
	}

	size_t lastDotPos = importTypeName.rfind( '.' );
	if( lastDotPos == std::string::npos )
	{
		pushError( loc, "type is in the same namespace and does not require importing" );
		return;
	}

	std::string localTypeName = importTypeName.substr( lastDotPos + 1 );

	// checks whether a type with the same local name was already imported
	ImportTypeMap::iterator it = _importedTypes.find( localTypeName );
	if( it != _importedTypes.end() )
	{
		PUSH_ERROR( loc, "import '" << importTypeName << "' conflicts with a "
			"previous import at line " << it->second.loc.begin.line );
		return;
	}

	_importedTypes.insert( ImportTypeMap::value_type( localTypeName, ImportInfo( importTypeName, loc ) ) );
}

void Loader::onNativeClass( const location& loc, const std::string& cppHeader, const std::string& cppType )
{
	// cppHeader comes wrapped between <>'s
	assert( cppHeader.length() >= 2 );
	CATCH_ERRORS( loc, _typeBuilder->defineNativeClass(
		cppHeader.substr( 1, cppHeader.length() - 2 ), cppType ) );
}

void Loader::onBaseType( const location& loc, const std::string& name )
{
	IType* type = resolveType( loc, name );
	if( getError() )
		return;

	if( !type )
	{
		PUSH_ERROR( loc, "could not load super-type '" << name << "'" );
	}
	else
	{
		CATCH_ERRORS( loc, _typeBuilder->defineBaseType( type ) );
	}
}

void Loader::onEnumIdentifier( const location& loc, const std::string& name )
{
	CATCH_ERRORS( loc, _typeBuilder->defineIdentifier( name ) );
	handleDocumentation( name );
}

void Loader::onPort( const location& loc, bool isFacet, const std::string& name )
{
	IType* type = getLastDeclaredType();
	if( getError() )
		return;

	if( type->getKind() != TK_INTERFACE )
	{
		pushError( loc, "an interface type was expected" );
		return;
	}

	CATCH_ERRORS( loc, _typeBuilder->definePort( name, static_cast<IInterface*>( type ), isFacet ) );
	handleDocumentation( name );
}

void Loader::onField( const location& loc, const std::string& name, bool isReadOnly )
{
	IType* type = getLastDeclaredType();
	if( getError() )
		return;

	CATCH_ERRORS( loc, _typeBuilder->defineField( name, type, isReadOnly ) );
	handleDocumentation( name );
}

void Loader::onMethod( const location& loc, const std::string& name )
{
	CATCH_ERRORS( loc, _methodBuilder = _typeBuilder->defineMethod( name ) );

	IType* returnType = getLastDeclaredType();
	if( getError() )
		return;

	if( returnType != NULL )
		CATCH_ERRORS( loc, _methodBuilder->defineReturnType( returnType ) );

	handleDocumentation( name );
}

void Loader::onParameter( const location& loc, bool isIn, bool isOut, const std::string& name )
{
	IType* type = getLastDeclaredType();
	if( getError() )
		return;

	CATCH_ERRORS( loc, _methodBuilder->defineParameter( name, type, isIn, isOut ) );
}

void Loader::onRaises( const location& loc, const std::string& name )
{
	IType* type = resolveType( loc, name );
	if( getError() )
		return;

	if( !type )
	{
		PUSH_ERROR( loc, "error loading exception type '" << name << "'" );
	}
	else if( type->getKind() != TK_EXCEPTION )
	{
		PUSH_ERROR( loc, "attempt to raise non-exception type '" << type->getFullName() << "'" );
	}
	else
	{
		CATCH_ERRORS( loc, _methodBuilder->defineException( static_cast<IException*>( type ) ) );
	}
}

void Loader::onEndMethod( const location& loc )
{
	CATCH_ERRORS( loc, _methodBuilder->createMethod() );
}

IType* Loader::getType()
{
	assert( _typeBuilder.isValid() );
	return _typeBuilder->createType();
}

IType* Loader::findImportedType( const std::string& alias )
{
	ImportTypeMap::iterator it = _importedTypes.find( alias );
	if( it != _importedTypes.end() )
		return it->second.type;
	return NULL;
}

void Loader::resolveImports()
{
	assert( _typeBuilder.isValid() );

	for( ImportTypeMap::iterator it = _importedTypes.begin(); it != _importedTypes.end(); ++it )
	{
		ImportInfo& ii = it->second;
		ii.type = resolveType( ii.loc, ii.typeName.c_str() );
		if( getError() )
			return;

		if( ii.type == NULL )
		{
			PUSH_ERROR( ii.loc, "could not import '" << ii.typeName << "'" );
			return;
		}
	}
}

void Loader::handleDocumentation( const std::string& member )
{
	_lastMember = member;

	if( _docBuffer.empty() )
		return;

	addDocumentation( member, _docBuffer );

	_docBuffer.clear();
}

IType* Loader::getLastDeclaredType()
{
	if( _lastDeclTypeName == "void" )
		return NULL;

	IType* lastDeclaredType = resolveType( _lastDeclTypeLocation, _lastDeclTypeName, _lastDeclTypeIsArray );
	if( !lastDeclaredType && !_lastDeclTypeIsArray )
	{
		PUSH_ERROR( _lastDeclTypeLocation, "error loading dependency '" << _lastDeclTypeName << "'" );
	}

	return lastDeclaredType;
}

} // namespace csl
} // namespace co
