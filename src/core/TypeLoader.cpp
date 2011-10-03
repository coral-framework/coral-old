/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "TypeLoader.h"
#include "TypeManager.h"
#include "TypeTransaction.h"
#include "utils/StringTokenizer.h"
#include <co/Coral.h>
#include <co/IType.h>
#include <co/IArray.h>
#include <co/Exception.h>
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodBuilder.h>
#include <co/reserved/OS.h>
#include <sstream>

namespace co {

// Root Loader Contructor:
TypeLoader::TypeLoader( const std::string& fullTypeName, ITypeManager* tm )
	: _fullTypeName( fullTypeName )
{
	_typeManager = static_cast<TypeManager*>( tm );
	_parentLoader = NULL;
	_namespace = NULL;
	_transaction = new TypeTransaction();
}

// Non-Root Loader Constructor:
TypeLoader::TypeLoader( const std::string& fullTypeName, TypeLoader* parent )
	: _fullTypeName( fullTypeName )
{
	_typeManager = parent->_typeManager;
	_parentLoader = parent;
	_namespace = parent->_namespace;
	_transaction = parent->_transaction;
}

TypeLoader::~TypeLoader()
{
	// empty
}

INamespace* getOrCreateChildNamespace( INamespace* parent, const std::string& name )
{
	INamespace* childNs = parent->getChildNamespace( name );
	if( childNs != NULL )
		return childNs;
	else
		return parent->defineChildNamespace( name );
}

IType* TypeLoader::loadType()
{
	try
	{
		std::string fullPath;
		std::string relativePath;
		if( findCSL( _fullTypeName, fullPath, relativePath ) )
		{
			INamespace* ns = _typeManager->getRootNS();

			// iterate over all subparts
			StringTokenizer st( relativePath, CORAL_OS_DIR_SEP_STR );
			st.nextToken();
			std::string currentToken = st.getToken();
			while( st.nextToken() )
			{
				ns = getOrCreateChildNamespace( ns, currentToken );
				currentToken = st.getToken();
			}

			_namespace = ns;

			if( parse( fullPath ) )
			{
				if( isRootLoader() )
					_transaction->commit();
				return getType();
			}
		}
	}
	catch( co::Exception& e )
	{
		pushError( e.getMessage() );
	}

	if( isRootLoader() )
		_transaction->rollback();

	return NULL;
}

ITypeBuilder* TypeLoader::createTypeBuilder( const std::string& typeName, TypeKind kind )
{
	return _namespace->defineType( typeName, kind, _transaction.get() );
}

IType* TypeLoader::resolveType( const csl::location& loc, const std::string& typeName, bool isArray )
{
	try
	{
		if( isArray )
		{
			IType* elementType = resolveType( loc, typeName );
			if( !elementType )
			{
				assert( getError() != NULL );
				pushError( loc, "error loading array element type" );
				return NULL;
			}
			return _typeManager->getArrayOf( elementType );
		}

		// try to find an imported type aliased as 'typeName'
		IType* type = findImportedType( typeName );
		if( type )
			return type;

		// try to find an existing type named 'typeName'
		type = findDependency( typeName );
		if( type )
			return type;

		// try to load a type named 'typeName'
		return loadDependency( typeName );
	}
	catch( co::Exception& e )
	{
		pushError( loc, e.getMessage() );
		return NULL;
	}
}

inline std::string formatTypeInNamespace( INamespace* ns, const std::string& typeName )
{
	const std::string& namespaceName = ns->getFullName();

	std::string str;
	str.reserve( namespaceName.length() + typeName.length() + 1 );
	str += namespaceName;
	str += '.';
	str += typeName;

	return str;
}

IType* TypeLoader::findDependency( const std::string& typeName )
{
	if( _namespace->getParentNamespace() )
	{
		// search _namespace if it is not the root namespace
		IType* type = _typeManager->findType( formatTypeInNamespace( _namespace, typeName ) );
		if( type )
			return type;
	}

	return _typeManager->findType( typeName );
}

IType* TypeLoader::loadDependency( const std::string& typeName )
{
	std::string fullPath;
	std::string relativePath;
	if( !findCSL( typeName, fullPath, relativePath ) )
		return NULL;

	// convert the relativePath into a full type name
	size_t length = relativePath.length() - 4;
	relativePath.resize( length ); // removes the trailing '.csl'
	for( size_t i = 0; i < length; ++i )
	{
		if( relativePath[i] == CORAL_OS_DIR_SEP )
			relativePath[i] = '.';
	}

	TypeLoader loader( relativePath, this );
	IType* type = loader.loadType();

	/*
		Set the current error as the child loader error (then it will be
		set as the inner error when this loader also detects the error).
	 */
	setError( loader.getError() );

	return type;
}

bool TypeLoader::findCSL( const std::string& typeName, std::string& fullPath, std::string& relativePath )
{
	std::string names[2];
	int n = 0;

	// if the current namespace is not the root, first try a relative path
	if( _namespace && _namespace->getParentNamespace() )
	{
		const std::string& fullNS = _namespace->getFullName();
		names[n].reserve( fullNS.size() + typeName.size() + 5 );
		names[n] = fullNS;
		names[n].push_back( '.' );
		names[n].append( typeName );
		OS::convertDotsToDirSeps( names[n] );
		names[n].append( ".csl" );
		++n;
	}

	names[n].reserve( typeName.size() + 4 );
	names[n] = typeName;
	OS::convertDotsToDirSeps( names[n] );
	names[n].append( ".csl" );
	++n;

	bool succeeded = OS::searchFile2( getPaths(), Range<const std::string>( names, n ),
											fullPath, NULL, &relativePath );
	if( !succeeded )
		CORAL_THROW( co::Exception, "type '" << typeName << "' was not found in the path" );

	return succeeded;
}

} // namespace co
