/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Parser.h"
#include "CSLLexer.h"
#include "CSLParser.h"
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodBuilder.h>
#include <co/IInterface.h>
#include <co/IException.h>
#include <co/TypeLoadException.h>
#include <co/ITypeTransaction.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/OS.h>
#include <sstream>

namespace co {
namespace csl {

Parser::Parser()
{
	_currentLine = 0;

	_lexer = NULL;
	_parser = NULL;
	_tokenStream = NULL;
	_inputFileStream = NULL;
}

Parser::~Parser()
{
	// NOTICE: order does matter below!

	if( _parser != NULL )
		_parser->free( _parser );

	if( _tokenStream != NULL )
		_tokenStream->free( _tokenStream );

	if( _lexer != NULL )
		_lexer->free( _lexer );

	if( _inputFileStream != NULL )
		_inputFileStream->close( _inputFileStream );
}

static void displayRecognitionError( ANTLR3_BASE_RECOGNIZER_struct*, pANTLR3_UINT8* )
{
	// empty, just to supress some AntLR messages
}

static void printToken( std::stringstream& sstream, pANTLR3_EXCEPTION ex, pANTLR3_UINT8* tokenNames )
{
	if( ex->expecting == ANTLR3_TOKEN_EOF )
		sstream << "<EOF>";
	else if( tokenNames == NULL )
		sstream << "token (" << ex->expecting << ")";
	else
		sstream << tokenNames[ex->expecting];
}

static void formatErrorMessage( pANTLR3_BASE_RECOGNIZER recognizer, std::string& message )
{
	std::stringstream sstream;

	pANTLR3_EXCEPTION ex = recognizer->state->exception;
	pANTLR3_UINT8* tokenNames = recognizer->state->tokenNames;

	sstream << "syntax error";
	assert( recognizer->type != ANTLR3_TYPE_TREE_PARSER );

	// print location
	if( recognizer->type == ANTLR3_TYPE_PARSER )
	{
		if( ex->charPositionInLine >= 0 )
			sstream << " at character " << ex->charPositionInLine;
		
		pANTLR3_COMMON_TOKEN theToken = reinterpret_cast<pANTLR3_COMMON_TOKEN>( ex->token );
		if( theToken != NULL )
		{
			if( theToken->type == ANTLR3_TOKEN_EOF )
				sstream << " at the end of file";
			else
			{
				pANTLR3_STRING ttext = theToken->getText( theToken );
				const char* chars = ( ttext == NULL ? "<no-text>" : (const char*)ttext->chars );

				// missing tokens are already described with the "expected" below
				if( strstr( chars, "<missing" ) == NULL )
					sstream << ", near '" << chars << "'";
			}
		}
	}

	// print cause
	switch( ex->type )
	{
	case ANTLR3_UNWANTED_TOKEN_EXCEPTION:
		sstream << ": extraneous input (expected ";
		printToken( sstream, ex, tokenNames );
		sstream << ")";
		break;

	case ANTLR3_MISSING_TOKEN_EXCEPTION:
		sstream << ": missing ";
		printToken( sstream, ex, tokenNames );
		break;

	case ANTLR3_MISMATCHED_TOKEN_EXCEPTION:
		sstream << ": expected ";
		printToken( sstream, ex, tokenNames );
		break;

	case ANTLR3_NO_VIABLE_ALT_EXCEPTION:
		sstream << ": cannot match to any predicted input";
		break;

	case ANTLR3_MISMATCHED_SET_EXCEPTION:
		sstream << ": unexpected input ";
		break;

	case ANTLR3_EARLY_EXIT_EXCEPTION:
		sstream << ": missing elements";
		break;

	default:
		sstream << ": syntax not recognized";
	}

	message = sstream.str();
}

static void handleError( pANTLR3_BASE_RECOGNIZER recognizer )
{	
	// indicate we're recovering from an error
	recognizer->state->errorRecovery = ANTLR3_TRUE;

	// indicate this recognizer had an error
	recognizer->state->errorCount++;
}

static void	reportParserError( pANTLR3_BASE_RECOGNIZER recognizer )
{
	// if already recovering from an error, ignore additional calls
	if( recognizer->state->errorRecovery == ANTLR3_TRUE )
		return;

	std::string message;
	formatErrorMessage( recognizer, message );

	handleError( recognizer );

	// access the parser
	Parser* parser = reinterpret_cast<Parser*>( recognizer->state->userp );
	parser->setCurrentLine( recognizer->state->exception->line );

	CORAL_THROW( TypeLoadException, message );
}

static void	reportLexerError( pANTLR3_BASE_RECOGNIZER recognizer )
{
	// if already recovering from an error, ignore additional calls
	if( recognizer->state->errorRecovery == ANTLR3_TRUE )
		return;

	std::string message;
	formatErrorMessage( recognizer, message );

	handleError( recognizer );

	// access the parser
	Parser* parser = reinterpret_cast<Parser*>( recognizer->state->userp );
	pANTLR3_LEXER lexer = reinterpret_cast<pANTLR3_LEXER>( recognizer->super );
	parser->setCurrentLine( lexer->getLine( lexer ) );

	CORAL_THROW( TypeLoadException, message );
}

void Parser::parse( const std::string& cslFilePath )
{
	// extract file's base name
	_cslFileBaseName = cslFilePath;
	
	// get file name without dir path
	size_t lastBarPos = _cslFileBaseName.rfind( CORAL_OS_DIR_SEP );
	if( lastBarPos != std::string::npos )
		_cslFileBaseName = _cslFileBaseName.substr( lastBarPos + 1 );
	
	// get file name without extension
	size_t lastDotPos = _cslFileBaseName.rfind( '.' );
	if( lastDotPos != std::string::npos )
		_cslFileBaseName = _cslFileBaseName.substr( 0, lastDotPos );
	
	// create 	
	_inputFileStream = antlr3AsciiFileStreamNew( (pANTLR3_UINT8)cslFilePath.c_str() );
	if( _inputFileStream == NULL )
		CORAL_THROW( IllegalArgumentException, "Error opening file '" << cslFilePath << "'" );

	_lexer = CSLLexerNew( _inputFileStream );
	assert( _lexer );

	_tokenStream = antlr3CommonTokenStreamSourceNew( ANTLR3_SIZE_HINT, TOKENSOURCE( _lexer ) );
	assert( _tokenStream );

	_parser = CSLParserNew( _tokenStream );
	assert( _parser );

	_parser->parser = this;

	_lexer->pLexer->rec->reportError = &reportLexerError;
	_parser->pParser->rec->reportError = &reportParserError;
	_lexer->pLexer->rec->displayRecognitionError = &displayRecognitionError;
	_parser->pParser->rec->displayRecognitionError = &displayRecognitionError;

	_parser->pParser->rec->state->userp = this;
	_lexer->pLexer->rec->state->userp = this;

	_parser->csl_file( _parser );

}

void Parser::onComment( const std::string& text )
{
	assert( text.length() >= 2 );

	static const char* START_END_BLANKS = " *\t\r\n";

	bool isRetroactive = false;
	bool isLineComment = ( text[1] != '*' );

	// discard the comment's opening symbol
	size_t start = 2;

	// is this retroactive documentation?
	if( text.length() > 2 && text[2] == '<' )
	{
		start = 3;
		isRetroactive = true;
	}
	start = text.find_first_not_of( START_END_BLANKS, start );
	
	// ignore a comment if it starts with a dash (useful for separators)
	if( text.length() > start && text[start] == '-' )
		return;

	// discard the comment's closing symbol (\n or */)
	size_t end = ( text.length() - ( isLineComment ? 1 : 2 ) );
	end = text.find_last_not_of( START_END_BLANKS, end );

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

void Parser::onCppBlock( const std::string& text )
{
	// removes the opening and closing c++ tags.
	addCppBlock( text.substr( 4, text.length() - 8 ) );
}

void Parser::onTypeSpecification( const std::string& typeName, TypeKind kind )
{
	if( _typeBuilder.isValid() )
	{
		CORAL_THROW( TypeLoadException, "only one type specification is allowed per file" );
	}

	if( typeName != _cslFileBaseName )
	{
		CORAL_THROW( TypeLoadException, "the name of the defined type must match its filename" );
	}

	_typeBuilder = createTypeBuilder( typeName, kind );

	handleDocumentation( std::string() );
	resolveImports();
}

void Parser::onTypeDeclaration( const std::string& name, bool isArray )
{
	_lastDeclaredTypeName = name;
	_lastDeclaredTypeIsArray = isArray;
	_lastDeclaredTypeLine = _currentLine;
}

void Parser::onImportClause( const std::string& importTypeName )
{
	/*
		Notice: the imported type cannot be loaded at this point because cyclic dependencies
		would cause a crash, as the current type's builder has not been created yet.
	 */

	if( _typeBuilder.isValid() )
		CORAL_THROW( TypeLoadException, "all import clauses must come before the type specification" );

	size_t lastDotPos = importTypeName.rfind( '.' );
	if( lastDotPos == std::string::npos )
		CORAL_THROW( TypeLoadException, "type '" << importTypeName
						<< "' is in the same namespace and does not require importing" );

	std::string localTypeName = importTypeName.substr( lastDotPos + 1 );

	// checks whether a type with the same local name was already imported
	ImportTypeMap::iterator it = _importedTypes.find( localTypeName );
	if( it != _importedTypes.end() )
		CORAL_THROW( TypeLoadException, "import '" << importTypeName
						<< "' conflicts with a previous import at line " << it->second.line  );

	_importedTypes.insert( ImportTypeMap::value_type( localTypeName, ImportInfo( importTypeName, _currentLine ) ) );
}

void Parser::onNativeClass( const std::string& cppHeader, const std::string& cppType )
{
	// cppHeader comes wrapped between <>'s
	assert( cppHeader.length() >= 2 );
	_typeBuilder->defineNativeClass( cppHeader.substr( 1, cppHeader.length() - 2 ), cppType );
}

void Parser::onSuperType( const std::string& name )
{
	IType* type = resolveType( name );
	if( !type )
		CORAL_THROW( TypeLoadException, "could not load super-type '" << name << "'" );

	_typeBuilder->defineSuperType( type );
}

void Parser::onIdentifierListItem( const std::string& name )
{
	_typeBuilder->defineIdentifier( name );
	handleDocumentation( name );
}

void Parser::onComponentInterface( bool isFacet, const std::string& name )
{
	IInterface* interface = dynamic_cast<IInterface*>( getLastDeclaredType() );
	if( !interface )
		CORAL_THROW( TypeLoadException, "an interface type was expected" );

	_typeBuilder->definePort( name, interface, isFacet );
	handleDocumentation( name );
}

void Parser::onField( const std::string& name, bool isReadOnly )
{
	_typeBuilder->defineField( name, getLastDeclaredType(), isReadOnly );
	handleDocumentation( name );
}

void Parser::onMethod( const std::string& name )
{
	_methodBuilder = _typeBuilder->defineMethod( name );

	IType* returnType = getLastDeclaredType();
	if( returnType != NULL )
		_methodBuilder->defineReturnType( returnType );

	handleDocumentation( name );
}

void Parser::onParameter( bool isIn, bool isOut, const std::string& name )
{
	_methodBuilder->defineParameter( name, getLastDeclaredType(), isIn, isOut );
}

void Parser::onExeptionRaised( const std::string& name )
{
	IType* type = resolveType( name );
	if( !type )
		CORAL_THROW( TypeLoadException, "error loading exception type '" << name << "'" );

	IException* exeptionType = dynamic_cast<IException*>( type );
	if( !exeptionType )
		CORAL_THROW( TypeLoadException, "attempt to raise non-exception type '" << type->getFullName() << "'" );

	_methodBuilder->defineException( exeptionType );
}

void Parser::onEndMethod()
{
	_methodBuilder->createMethod();
}

IType* Parser::getType()
{
	assert( _typeBuilder.isValid() );
	return _typeBuilder->createType();
}

IType* Parser::findImportedType( const std::string& alias )
{
	ImportTypeMap::iterator it = _importedTypes.find( alias );
	if( it != _importedTypes.end() )
		return it->second.type;
	return NULL;
}

void Parser::resolveImports()
{
	assert( _typeBuilder.isValid() );

	int originalLine = getCurrentLine();

	for( ImportTypeMap::iterator it = _importedTypes.begin(); it != _importedTypes.end(); ++it )
	{
		// set the current line: all imports are loaded after being parsed, when the type definition is found
		// so and error at any import would show the wrong line number. So, it is necessary to set the line
		// at which the import was defined. The current line is saved to be restored later.
		setCurrentLine( it->second.line );

		it->second.type = resolveType( it->second.typeName.c_str() );
		if( it->second.type == NULL )
			CORAL_THROW( TypeLoadException, "could not import '" << it->second.typeName << "'" );
	}

	// if no exceptions are raised, restore the original line.
	setCurrentLine( originalLine );
}

void Parser::handleDocumentation( const std::string& member )
{
	_lastMember = member;

	if( _docBuffer.empty() )
		return;

	addDocumentation( member, _docBuffer );

	_docBuffer.clear();
}

IType* Parser::getLastDeclaredType()
{
	IType* lastDeclaredType;
	if( _lastDeclaredTypeName == "void" )
	{
		lastDeclaredType = NULL;
	}
	else
	{
		lastDeclaredType = resolveType( _lastDeclaredTypeName, _lastDeclaredTypeIsArray );
		if( !lastDeclaredType )
			CORAL_THROW( TypeLoadException, "error loading dependency '" << _lastDeclaredTypeName << "'" );
	}
	return lastDeclaredType;
}

} // namespace csl
} // namespace co
