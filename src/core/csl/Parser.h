/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CSL_PARSER_H_
#define _CSL_PARSER_H_

#include <co/System.h>
#include <co/RefPtr.h>
#include <co/TypeKind.h>
#include <co/TypeBuilder.h>
#include <co/MethodBuilder.h>

#include <map>
#include <string>

// AntLR forward declarations:
typedef struct CSLLexer_Ctx_struct* pCSLLexer;
typedef struct CSLParser_Ctx_struct* pCSLParser;
typedef struct ANTLR3_INPUT_STREAM_struct* pANTLR3_INPUT_STREAM;
typedef struct ANTLR3_COMMON_TOKEN_STREAM_struct* pANTLR3_COMMON_TOKEN_STREAM;

namespace co {
	class Type;
	class Namespace;
	class TypeCreationTransaction;
} // namespace co

namespace csl {

class Parser
{
public:
	Parser();
	virtual ~Parser();

	//! Parses the given file.
	void parse( const std::string& cslFilePath );

	//! Returns the current line number, as reported by the parser.
	inline co::uint32 getCurrentLine() { return _currentLine; }

	//! Updates the current line number. Errors are reported at this position.
	inline void setCurrentLine( co::uint32 line ) { _currentLine = line; }

	//! Parser events
	void onComment( const std::string& text );
	void onCppBlock( const std::string& text );
	void onTypeSpecification( const std::string& typeName, co::TypeKind kind );
	void onTypeDeclaration( const std::string& qualifiedId, bool isArray );
	void onImportClause( const std::string& importTypeName );
	void onNativeClass( const std::string& cppType, const std::string& cppHeader );
	void onIdentifierListItem( const std::string& name );
	void onSuperType( const std::string& typeName );
	void onStructMember( const std::string& memberIdentifier );
	void onComponentInterface( bool isProvided, const std::string& name );
	void onAttribute( bool isReadOnly, const std::string& name );
	void onMethod( const std::string& name );
	void onParameter( bool isIn, bool isOut, const std::string& name );
	void onExeptionRaised( const std::string& name );
	void onEndMethod();

protected:
	//! Template method: creates the TypeBuilder for the type being parsed.
	virtual co::TypeBuilder* createTypeBuilder( const std::string& typeName, co::TypeKind kind ) = 0;

	//! Template method: resolves a type given its \c typeName and whether it is an array.
	//! \param typeName may be a full or relative type name.
	virtual co::Type* resolveType( const std::string& typeName, bool isArray = false ) = 0;

	/*!
		Template method: adds the passed \c text as documentation for the specified \c member of the
		parsed type. When \c member is an empty string, documentation is for the parsed type itself.
	 */
	virtual void addDocumentation( const std::string& member, const std::string& text ) = 0;

	//!	Template method: adds the passed \c text as a c++ block to be used by the type that is beeing defined.
	virtual void addCppBlock( const std::string& text ) = 0;

	//! Returns the type created by our TypeBuilder.
	//! Notice that onTypeSpecification() should have been called before.
	co::Type* getType();

	//! Retrieves an imported co::Type* given its local type name (i.e. its alias).
	//! Returns NULL if the alias does not identify an imported type.
	co::Type* findImportedType( const std::string& alias );

	//! Sets whether comments should be processed. When this is true (default) addDocumentation() is never called.
	inline void setIgnoreComments( bool ignore ) { _ignoreComments = ignore; }

	//! Sets whether c++ blocks should be processed. When this is true (default) addCppBlock() is never called.
	inline void setIgnoreCppBlocks( bool ignore ) { _ignoreCppBlocks = ignore; }

private:
	struct ImportInfo
	{
		ImportInfo( const std::string& typeName, co::uint32 line )
			: typeName( typeName ), line( line ), type( NULL )
		{;}

		std::string typeName;	// full or relative type name
		co::uint32 line;		// line number of the import
		co::Type* type;			// resolved type (only after calling resolveImports())
	};

	typedef std::map<std::string, ImportInfo> ImportTypeMap;
	ImportTypeMap _importedTypes;

	//! Resolves all types added to the _importedTypes map. This method should be called
	//! after the creation of the type builder to avoid problems with cyclic dependencies.
	void resolveImports();

	/*
		Adds any buffered (pre-)documentation to the specified member
			(or to the type, when a member is not specified).
		Also updates _lastMember so any post-doc goes to this member from now on.
	 */
	void handleDocumentation( const std::string& member );

	// formats the passed input removing any '\r' character 
	void filterText( const std::string& input, std::string& output );

	co::Type* getLastDeclaredType();

private:
	co::uint32 _currentLine;
	co::RefPtr<co::TypeBuilder> _typeBuilder;
	co::RefPtr<co::MethodBuilder> _methodBuilder;

	pCSLLexer _lexer;
	pCSLParser _parser;
	pANTLR3_INPUT_STREAM _inputFileStream;
	pANTLR3_COMMON_TOKEN_STREAM _tokenStream;

	std::string _lastDeclaredTypeName;
	co::uint32 _lastDeclaredTypeLine;
	bool _lastDeclaredTypeIsArray;

	// the base name of the file being parsed
	std::string _cslFileBaseName;

	// first error
	int _errorLine;
	bool _errorOccurred;
	std::string _errorMessage;

	bool _ignoreComments;		// if true (default) comments are ignored and doc is not processed
	bool _ignoreCppBlocks;		// if true (default) c++ blocks are ignored
	std::string _docBuffer;		// accumulates 'pre'-documentation
	std::string _lastMember;	// keeps track of the last defined member
};

} // namespace csl

#endif // _CSL_PARSER_H_
