/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeLoader.h"
#include "TypeManager.h"
#include "TypeCreationTransaction.h"
#include "tools/StringTokenizer.h"
#include <co/IType.h>
#include <co/IArrayType.h>
#include <co/Exception.h>
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/IMethodBuilder.h>
#include <co/reserved/OS.h>
#include <sstream>

namespace co {

// Root Loader Contructor:
TypeLoader::TypeLoader( const std::string& fullTypeName,
						ArrayRange<const std::string> path,
						ITypeManager* tm )
	: _fullTypeName( fullTypeName ), _path( path )
{
	assert( dynamic_cast<TypeManager*>( tm ) );
	_typeManager = static_cast<TypeManager*>( tm );

	_parentLoader = NULL;
	_namespace = NULL;
	_transaction = new TypeCreationTransaction();
}

// Non-Root Loader Constructor:
TypeLoader::TypeLoader( const std::string& fullTypeName, TypeLoader* parent )
	: _fullTypeName( fullTypeName ), _path( parent->_path )
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
	std::string fullPath;
	std::string relativePath;
	if( !findCSL(_fullTypeName, fullPath, relativePath ) )
	{
		if( isRootLoader() )
			_transaction->rollback();

		return NULL;
	}

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

	try
	{
		parse( fullPath );

		setCurrentLine( -1 );

	   	if( isRootLoader() )
			_transaction->commit();

		return getType();
	}
	catch( Exception& e )
	{
		// create a csl error, using the current one as inner error
		_cslError = new csl::Error( e.getMessage(), fullPath, getCurrentLine(), _cslError.get() );

		if( isRootLoader() )
			_transaction->rollback();
	}

	return NULL;
}

ITypeBuilder* TypeLoader::createTypeBuilder( const std::string& typeName, TypeKind kind )
{
	return _namespace->defineType( typeName, kind, _transaction.get() );
}

IType* TypeLoader::resolveType( const std::string& typeName, bool isArray )
{
	if( isArray )
	{
		IType* elementType = resolveType( typeName );
		if( !elementType )
			CORAL_THROW( Exception, "error loading array element type '" << typeName << "'" );
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

void TypeLoader::addDocumentation( const std::string& member, const std::string& text )
{
	// DocMap keys are like 'name.space.IType:memberName'
	std::string key;
	key.reserve( _fullTypeName.length() + member.length() + 1 );
	key += _fullTypeName;
	if( !member.empty() )
	{
		key += ":";
		key += member;
	}

	_typeManager->addDocumentation( key, text );
}

void TypeLoader::addCppBlock( const std::string& text )
{
	_typeManager->addCppBlock( _fullTypeName, text );
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

	// set the current error as the child loader error (then it will be set as the inner error
	// when this loader also detects the error)
	_cslError = loader.getError();

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

	bool succeeded = OS::searchFile2( _path, ArrayRange<const std::string>( names, n ),
											fullPath, NULL, &relativePath );
	if( !succeeded )
	{
		std::string message( "type '" );
		message.append( typeName );
		message.append( "' was not found in the path" );
		_cslError = new csl::Error( message, "", -1, NULL );
	}

	return succeeded;
}

} // namespace co
