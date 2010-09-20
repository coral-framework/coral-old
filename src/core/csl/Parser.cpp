/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Parser.h"
#include "CSLLexer.h"
#include "CSLParser.h"
#include <co/Namespace.h>
#include <co/TypeBuilder.h>
#include <co/CoreException.h>
#include <co/MethodBuilder.h>
#include <co/InterfaceType.h>
#include <co/ExceptionType.h>
#include <co/TypeCreationTransaction.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

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
	// signal we are in error recovery now
	recognizer->state->errorRecovery = ANTLR3_TRUE;

	// indicate this recognizer had an error while processing.
	recognizer->state->errorCount++;
}

static void	reportParserError( pANTLR3_BASE_RECOGNIZER recognizer )
{
	if( recognizer->state->errorRecovery == ANTLR3_TRUE )
	{
		// already in error recovery so don't display another error while doing so
		return;
	}

	std::string message;
	formatErrorMessage( recognizer, message );

	handleError( recognizer );
	// access the parser
	Parser* parser = reinterpret_cast<Parser*>( recognizer->state->userp );
	parser->setCurrentLine( recognizer->state->exception->line );

	CORAL_THROW( co::CoreException, message );
}

static void	reportLexerError( pANTLR3_BASE_RECOGNIZER recognizer )
{
	if( recognizer->state->errorRecovery == ANTLR3_TRUE )
	{
		// already in error recovery so don't display another error while doing so
		return;
	}
	handleError( recognizer );

	std::string message;
	formatErrorMessage( recognizer, message );

	handleError( recognizer );
	// access the parser
	Parser* parser = reinterpret_cast<Parser*>( recognizer->state->userp );
	pANTLR3_LEXER lexer = reinterpret_cast<pANTLR3_LEXER>( recognizer->super );
	parser->setCurrentLine( lexer->getLine( lexer ) );

	CORAL_THROW( co::CoreException, message );
}

void Parser::parse( const std::string& cslFilePath )
{
	// extract file's base name
	_cslFileBaseName = cslFilePath;
	
	// get file name without dir path
	std::size_t lastBarPos = _cslFileBaseName.rfind( '/' );
	if( lastBarPos != std::string::npos )
		_cslFileBaseName = _cslFileBaseName.substr( lastBarPos + 1 );
	
	// get file name without extension
	std::size_t lastDotPos = _cslFileBaseName.rfind( '.' );
	if( lastDotPos != std::string::npos )
		_cslFileBaseName = _cslFileBaseName.substr( 0, lastDotPos );
	
	// create 	
	_inputFileStream = antlr3AsciiFileStreamNew( (pANTLR3_UINT8)cslFilePath.c_str() );
	if( _inputFileStream == NULL )
		CORAL_THROW( co::IllegalArgumentException, "Error opening file '" << cslFilePath << "'" );

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
	std::string docText;
	filterText( text, docText );

	std::size_t carriagePos = docText.find( '\r' );
	while ( carriagePos != std::string::npos )
	{
		docText.erase( carriagePos, 1 );
		carriagePos = docText.find( '\r' );
	}

	// is this retroactive documentation?
	if( docText.length() > 2 && docText[2] == '<' )
	{
		addDocumentation( _lastMember, docText );
		return;
	}

	_docBuffer.append( docText );
}

void Parser::onCppBlock( const std::string& text )
{
	std::string cppText;
	filterText( text, cppText );

	//removes the opening and closing c++ tags.
	addCppBlock( cppText.substr( 4, cppText.length() - 8 ) );
}

void Parser::onTypeSpecification( const std::string& typeName, co::TypeKind kind )
{
	if( _typeBuilder.isValid() )
	{
		CORAL_THROW( co::CoreException, "only one type specification is allowed per file" );
	}

	if( typeName != _cslFileBaseName )
	{
		CORAL_THROW( co::CoreException, "the name of the defined type must match its filename" );
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
	{
		CORAL_THROW( co::CoreException, "all import clauses must come before the type specification" );
	}

	std::size_t lastDotPos = importTypeName.rfind( '.' );
	if( lastDotPos == std::string::npos )
	{
		CORAL_THROW( co::CoreException, "type '" << importTypeName << "' is in the same namespace and does not require importing" );
	}

	std::string localTypeName = importTypeName.substr( lastDotPos + 1 );

	// checks whether a type with the same local name was already imported
	ImportTypeMap::iterator it = _importedTypes.find( localTypeName );
	if( it != _importedTypes.end() )
	{
		CORAL_THROW( co::CoreException, "import '" << importTypeName << "' conflicts with a previous import at line " << it->second.line  );
	}

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
	co::Type* type = resolveType( name );
	if( !type )
		CORAL_THROW( co::CoreException, "could not load super-type '" << name << "'" );

	_typeBuilder->defineSuperType( type );
}

void Parser::onStructMember( const std::string& memberIdentifier )
{
	_typeBuilder->defineAttribute( memberIdentifier, getLastDeclaredType(), false );
	handleDocumentation( memberIdentifier );
}

void Parser::onIdentifierListItem( const std::string& name )
{
	_typeBuilder->defineIdentifier( name );
	handleDocumentation( name );
}

void Parser::onComponentInterface( bool isProvided, const std::string& name )
{
	co::InterfaceType* interface = dynamic_cast<co::InterfaceType*>( getLastDeclaredType() );
	if( !interface )
		CORAL_THROW( co::CoreException, "an interface type was expected" );

	_typeBuilder->defineInterface( name, interface, isProvided );
	handleDocumentation( name );
}

void Parser::onAttribute( bool isReadOnly, const std::string& name )
{
	_typeBuilder->defineAttribute( name, getLastDeclaredType(), isReadOnly );
	handleDocumentation( name );
}

void Parser::onMethod( const std::string& name )
{
	_methodBuilder = _typeBuilder->defineMethod( name );

	co::Type* returnType = getLastDeclaredType();
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
	co::Type* type = resolveType( name );
	if( !type )
		CORAL_THROW( co::CoreException, "error loading exception type '" << name << "'" );

	co::ExceptionType* exeptionType = dynamic_cast<co::ExceptionType*>( type );
	if( !exeptionType )
		CORAL_THROW( co::CoreException, "attempt to raise non-exception type '" << type->getFullName() << "'" );

	_methodBuilder->defineException( exeptionType );
}

void Parser::onEndMethod()
{
	_methodBuilder->createMethod();
}

co::Type* Parser::getType()
{
	assert( _typeBuilder.isValid() );
	return _typeBuilder->createType();
}

co::Type* Parser::findImportedType( const std::string& alias )
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
			CORAL_THROW( co::CoreException, "could not import '" << it->second.typeName << "'" );
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

void Parser::filterText( const std::string& input, std::string& output )
{
	// remove '\r's from the text
	size_t length = input.length();
	output.reserve( length );
	for( size_t i = 0; i < length; ++i )
		if( input[i] != '\r' )
			output.push_back( input[i] );
}

co::Type* Parser::getLastDeclaredType()
{
	co::Type* lastDeclaredType;
	if( _lastDeclaredTypeName == "void" )
	{
		lastDeclaredType = NULL;
	}
	else
	{
		lastDeclaredType = resolveType( _lastDeclaredTypeName, _lastDeclaredTypeIsArray );
		if( !lastDeclaredType )
			CORAL_THROW( co::CoreException, "error loading dependency '" << _lastDeclaredTypeName << "'" );
	}
	return lastDeclaredType;
}

} // namespace csl
