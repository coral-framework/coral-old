/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_CSL_LOADER_H_
#define _CO_CSL_LOADER_H_

#include "Error.h"
#include <co/RefPtr.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodBuilder.h>
#include <map>
#include <deque>
#include <string>

namespace co {

class IType;
class INamespace;
class ITypeTransaction;

namespace csl {

class Loader
{
public:
	Loader();
	virtual ~Loader();

	/*!
		Parses the given file. Never throws.
		Returns true on success, or false if there were errors.
	 */
	bool parse( const std::string& filename );

	//! Error handling:
	inline Error* getError() { return _error.get(); }
	inline const std::string* getCurrentFileName() { return _filename; }
	void pushError( const location& loc, const std::string& message );
	void pushError( const std::string& filename, const std::string& message );
	void pushError( const std::string& message );
	void setError( Error* error );

	//! Memory management:
	std::string* newString();
	std::string* newString( const char* cstr );

	//! Parser events
	void onComment( const location& loc, const std::string& text );
	void onCppBlock( const location& loc, const std::string& text );
	void onTypeSpec( const location& specLoc, const location& nameLoc,
						const std::string& typeName, TypeKind kind );
	void onTypeDecl( const location& loc, const std::string& qualifiedId, bool isArray );
	void onImport( const location& loc, const std::string& importTypeName );
	void onNativeClass( const location& loc, const std::string& cppType, const std::string& cppHeader );
	void onEnumIdentifier( const location& loc, const std::string& name );
	void onBaseType( const location& loc, const std::string& typeName );
	void onPort( const location& loc, bool isFacet, const std::string& name );
	void onField( const location& loc, const std::string& name, bool isReadOnly );
	void onMethod( const location& loc, const std::string& name );
	void onParameter( const location& loc, bool isIn, bool isOut, const std::string& name );
	void onRaises( const location& loc, const std::string& name );
	void onEndMethod( const location& loc );

protected:
	//! Template method: creates the ITypeBuilder for the type being parsed.
	virtual ITypeBuilder* createTypeBuilder( const std::string& typeName, TypeKind kind ) = 0;

	//! Template method: resolves a type given its \a typeName and whether it is an array.
	//! \param typeName may be a full or relative type name.
	virtual IType* resolveType( const location& loc, const std::string& typeName, bool isArray = false ) = 0;

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
		ImportInfo( const std::string& typeName, const location& location )
			: typeName( typeName ), loc( location ), type( NULL )
		{;}

		std::string typeName;	// full or relative type name
		location loc;			// location of the import clause
		IType* type;			// resolved type (only after calling resolveImports())
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
	const std::string* _filename;
	RefPtr<Error> _error;

	// base name of the CSL file (should match the type name)
	std::string _cslFileBaseName;

	RefPtr<ITypeBuilder> _typeBuilder;
	RefPtr<IMethodBuilder> _methodBuilder;
	
	bool _lastDeclTypeIsArray;
	std::string _lastDeclTypeName;
	location _lastDeclTypeLocation;

	// keeps track of the last defined member
	std::string _lastMember;

	// accumulates 'pre'-documentation
	std::string _docBuffer;

	// memory pools
	std::deque<std::string> _stringPool;
};

} // namespace csl
} // namespace co

#endif // _CO_CSL_LOADER_H_
