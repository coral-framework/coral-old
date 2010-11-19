/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPELOADER_H_
#define _CO_TYPELOADER_H_

#include "csl/Error.h"
#include "csl/Parser.h"
#include "tools/RefCounted.h"
#include <co/RefPtr.h>
#include <co/TypeKind.h>
#include <co/Platform.h>
#include <co/ArrayRange.h>
#include <co/TypeCreationTransaction.h>

namespace co {
	class Type;
	class Namespace;
	class TypeBuilder;
} // namespace co

class TypeManager;

/*!
	Locates and loads a co::Type from a CSL file in the filesystem.

	The type is identified by the fully-qualified \a typeName passed in the
	constructor. The CSL file where the type is specified should reside in one of
	the type repositories specified in the 'path' parameter, also in the constructor.

	Please notice that a single call to loadType() may cause multiple types to be loaded,
	since all dependencies of the loaded type will be automatically loaded in a
	depth-first, recursive manner.
 
	All search for dependencies and type creation is done within the context of a given
	co::TypeManager, which should also be passed in the constructor.
 */
class CORAL_EXPORT TypeLoader : private csl::Parser
{
public:
	/*!
		Constructs a loader for the type with the given \a fullTypeName.
		The type's CSL file is located using the given \a path (a list of type repositories).
		Types are looked up and created within the context of the specified \a typeManager.
	 */
	TypeLoader( const std::string& fullTypeName,
				co::ArrayRange<const std::string> path,
				co::TypeManager* typeManager );

	//! Destructor.
	virtual ~TypeLoader();

	//! Retrieves the error occurred during the loading process.
	//! Returns NULL if the loading was successful.
	inline csl::Error* getError() { return _cslError.get(); }

	/*!
		Effectively loads the type (and its dependencies) from the filesystem.

		The CSL file is parsed and any type dependencies are loaded	recursively. If the
		file is found in the filesystem and no error occurs while parsing the CSL files,
		the loaded co::Type	instance is returned. Otherwise, this method returns NULL, and
		the exact error stack can be obtained through the getError() method.

		\warning If any error occurs while loading a type, this method returns NULL
			and all partially constructed types are rolled back.

	 */
	co::Type* loadType();

private:
	/*
		Private contructor: creates a 'non-root' loader that is used for loading
		type dependencies recursively. In this case, we re-use the _docMap, _path
		and _transaction from our parent.
	 */
	TypeLoader( const std::string& fullTypeName, TypeLoader* parent );

	// Returns whether this is a root loader.
	inline bool isRootLoader() const { return _parentLoader == NULL; }

	// Template method implementation: instantiates a TypeBuilder for use by the Parser.
	virtual co::TypeBuilder* createTypeBuilder( const std::string& typeName, co::TypeKind kind );

	/*
		Template method implementation: finds or loads a type dependency, given the
		type's full or relative name. Here's the algorithm:
		  1) search for an imported type aliased as 'typeName';
		  2) search for a type named 'typeName' in the current namespace;
		  3) search for a type named 'typeName' in the root namespace;
		  4) try to load the type using the current path and returns NULL in case
			 of error parsing the CSL file or if the file is not found in the
			 filesystem (use getError() to access the error stack).
	 */
	virtual co::Type* resolveType( const std::string& typeName, bool isArray = false );

	/*
		Processes a documentation chunk. If the \a member parameter is not specified, the
		documentation is associated with the type itself. If a member already contains some
		documentation, the extra docs are appended to previous contents.
	 */
	void addDocumentation( const std::string& member, const std::string& documentation );

	// Appends the passed \a text to the CppBlockMap of our type.
	void addCppBlock( const std::string& text );

	//	Searches an existing type with the passed \a typeName. The name can be fully qualified
	//	or relative to the current type's location. Returns NULL if the type is not found.
	co::Type* findDependency( const std::string& typeName );

	/*
		Tries to load the type with the passed \a typeName. The name can be fully qualified
		or relative to the current type's location. An exception is thrown if the type's CSL
		file is not found. Otherwise, this method may return NULL if there's an error parsing
		the CSL file.
	 */
	co::Type* loadDependency( const std::string& typeName );

	// Searches the csl file within the current coral path and return true if the file was found.
	bool findCslFile( const std::string& typeName, std::string& cslFilePath, std::string& foundRelativePath );

private:
	std::string _fullTypeName;
	const co::ArrayRange<const std::string> _path;
	TypeManager* _typeManager;
	TypeLoader* _parentLoader;

	co::Namespace* _namespace;
	co::RefPtr<co::TypeCreationTransaction> _transaction;

	co::RefPtr<csl::Error> _cslError;
};

#endif
