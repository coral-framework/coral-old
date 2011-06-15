/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_CSL_PARSER_H_
#define _CO_CSL_PARSER_H_

#include <co/RefPtr.h>
#include <co/TypeKind.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodBuilder.h>
#include <map>
#include <string>

// AntLR forward declarations:
typedef struct CSLLexer_Ctx_struct* pCSLLexer;
typedef struct CSLParser_Ctx_struct* pCSLParser;
typedef struct ANTLR3_INPUT_STREAM_struct* pANTLR3_INPUT_STREAM;
typedef struct ANTLR3_COMMON_TOKEN_STREAM_struct* pANTLR3_COMMON_TOKEN_STREAM;

namespace co {

class IType;
class INamespace;
class ITypeTransaction;

namespace csl {

class Parser
{
public:
	Parser();
	virtual ~Parser();

	//! Parses the given file.
	void parse( const std::string& cslFilePath );

	//! Returns the current line number, as reported by the parser.
	inline uint32 getCurrentLine() { return _currentLine; }

	//! Updates the current line number. Errors are reported at this position.
	inline void setCurrentLine( uint32 line ) { _currentLine = line; }

	//! Parser events
	void onComment( const std::string& text );
	void onCppBlock( const std::string& text );
	void onTypeSpecification( const std::string& typeName, TypeKind kind );
	void onTypeDeclaration( const std::string& qualifiedId, bool isArray );
	void onImportClause( const std::string& importTypeName );
	void onNativeClass( const std::string& cppType, const std::string& cppHeader );
	void onIdentifierListItem( const std::string& name );
	void onBaseType( const std::string& typeName );
	void onComponentInterface( bool isFacet, const std::string& name );
	void onField( const std::string& name, bool isReadOnly );
	void onMethod( const std::string& name );
	void onParameter( bool isIn, bool isOut, const std::string& name );
	void onExeptionRaised( const std::string& name );
	void onEndMethod();

protected:
	//! Template method: creates the ITypeBuilder for the type being parsed.
	virtual ITypeBuilder* createTypeBuilder( const std::string& typeName, TypeKind kind ) = 0;

	//! Template method: resolves a type given its \a typeName and whether it is an array.
	//! \param typeName may be a full or relative type name.
	virtual IType* resolveType( const std::string& typeName, bool isArray = false ) = 0;

	/*!
		Template method: adds the passed \a text as documentation for the specified \a member of the
		parsed type. When \a member is an empty string, documentation is for the parsed type itself.
	 */
	virtual void addDocumentation( const std::string& member, const std::string& text ) = 0;

	//!	Template method: adds the passed \a text as a c++ block to be used by the type that is beeing defined.
	virtual void addCppBlock( const std::string& text ) = 0;

	//! Returns the type created by our ITypeBuilder.
	//! Notice that onTypeSpecification() should have been called before.
	IType* getType();

	//! Retrieves an imported IType* given its local type name (i.e. its alias).
	//! Returns NULL if the alias does not identify an imported type.
	IType* findImportedType( const std::string& alias );

private:
	struct ImportInfo
	{
		ImportInfo( const std::string& typeName, uint32 line )
			: typeName( typeName ), line( line ), type( NULL )
		{;}

		std::string typeName;	// full or relative type name
		uint32 line;			// line number of the import
		IType* type;				// resolved type (only after calling resolveImports())
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

	IType* getLastDeclaredType();

private:
	uint32 _currentLine;
	RefPtr<ITypeBuilder> _typeBuilder;
	RefPtr<IMethodBuilder> _methodBuilder;

	pCSLLexer _lexer;
	pCSLParser _parser;
	pANTLR3_INPUT_STREAM _inputFileStream;
	pANTLR3_COMMON_TOKEN_STREAM _tokenStream;

	std::string _lastDeclaredTypeName;
	uint32 _lastDeclaredTypeLine;
	bool _lastDeclaredTypeIsArray;

	// the base name of the file being parsed
	std::string _cslFileBaseName;

	// keeps track of the last defined member
	std::string _lastMember;

	// accumulates 'pre'-documentation
	std::string _docBuffer;
};

} // namespace csl
} // namespace co

#endif // _CO_CSL_PARSER_H_
