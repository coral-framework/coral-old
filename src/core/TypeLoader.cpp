/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeLoader.h"
#include "TypeManager.h"
#include "TypeCreationTransaction.h"
#include "tools/StringTokenizer.h"
#include <co/Type.h>
#include <co/ArrayType.h>
#include <co/Exception.h>
#include <co/Namespace.h>
#include <co/TypeBuilder.h>
#include <co/MethodBuilder.h>
#include <co/reserved/FileLookUp.h>
#include <sstream>

// Root Loader Contructor:
TypeLoader::TypeLoader( const std::string& fullTypeName, co::ArrayRange<const std::string> path, co::TypeManager* tm )
	: _fullTypeName( fullTypeName ), _path( path )
{
	_typeManager = dynamic_cast<TypeManager*>( tm );
	assert( _typeManager );

	_parentLoader = NULL;
	_namespace = NULL;
	_transaction = new ::TypeCreationTransaction();
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

co::Namespace* getOrCreateChildNamespace( co::Namespace* parent, const std::string& name )
{
	co::Namespace* childNs = parent->getChildNamespace( name );
	if( childNs != NULL )
		return childNs;
	else
		return parent->defineChildNamespace( name );
}

co::Type* TypeLoader::loadType()
{
	std::string cslPath;
	std::string foundRelativePath;
	if( !findCslFile(_fullTypeName, cslPath, foundRelativePath ) )
	{
		if( isRootLoader() )
			_transaction->rollback();

		return NULL;
	}

	co::Namespace* ns = _typeManager->getRootNS();

	// iterate over all subparts
	co::StringTokenizer st( foundRelativePath, "/" );
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
		parse( cslPath );

		setCurrentLine( -1 );

	   	if( isRootLoader() )
			_transaction->commit();

		return getType();
	}
	catch( co::Exception& e )
	{
		// create a csl error, using the current one as inner error
		_cslError = new csl::Error( e.getMessage(), cslPath, getCurrentLine(), _cslError.get() );

		if( isRootLoader() )
			_transaction->rollback();
	}

	return NULL;
}

co::TypeBuilder* TypeLoader::createTypeBuilder( const std::string& typeName, co::TypeKind kind )
{
	return _namespace->defineType( typeName, kind, _transaction.get() );
}

co::Type* TypeLoader::resolveType( const std::string& typeName, bool isArray )
{
	if( isArray )
	{
		co::Type* elementType = resolveType( typeName );
		if( !elementType )
			CORAL_THROW( co::Exception, "error loading array element type '" << typeName << "'" );
		return _typeManager->getArrayOf( elementType );
	}

	// try to find an imported type aliased as 'typeName'
	co::Type* type = findImportedType( typeName );
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
	// DocMap keys are like 'name.space.Type:memberName'	
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

inline std::string formatTypeInNamespace( co::Namespace* ns, const std::string& typeName )
{
	const std::string& namespaceName = ns->getFullName();

	std::string str;
	str.reserve( namespaceName.length() + typeName.length() + 1 );
	str += namespaceName;
	str += '.';
	str += typeName;

	return str;
}

co::Type* TypeLoader::findDependency( const std::string& typeName )
{
	if( _namespace->getParentNamespace() )
	{
		// search _namespace if it is not the root namespace
		co::Type* type = _typeManager->findType( formatTypeInNamespace( _namespace, typeName ) );
		if( type )
			return type;
	}

	return _typeManager->findType( typeName );
}

co::Type* TypeLoader::loadDependency( const std::string& typeName )
{
	std::string cslPath;
	std::string foundFilePath;
	if( !findCslFile( typeName, cslPath, foundFilePath ) )
		return NULL;

	// convert slashes to dots in foundFilePath
	std::size_t length = foundFilePath.length();
	for( std::size_t i = 0; i < length; ++i )
	{
		if( foundFilePath[i] == '/' )
			foundFilePath[i] = '.';
	}

	TypeLoader loader( foundFilePath, this );
	co::Type* type = loader.loadType();

	// set the current error as the child loader error (then it will be set as the inner error
	// when this loader also detects the error)
	_cslError = loader.getError();

	return type;
}

bool TypeLoader::findCslFile( const std::string& typeName, std::string& cslFilePath, std::string& foundRelativePath )
{
	static std::vector<std::string> s_validExtensions;

	if( s_validExtensions.empty() )
		s_validExtensions.push_back( "csl" );

	co::FileLookUp fileLookUp( _path, s_validExtensions );

	// if the current namespace is not the root, first try a relative path
	if( _namespace && _namespace->getParentNamespace() )
		fileLookUp.addFilePath( formatTypeInNamespace( _namespace, typeName ), true );

	fileLookUp.addFilePath( typeName, true );

	bool succeeded = fileLookUp.locate( cslFilePath, NULL, &foundRelativePath );
	if( !succeeded )
	{
		std::string message( "type '" );
		message.append( typeName );
		message.append( "' was not found in the path" );
		_cslError = new csl::Error( message, "", -1, NULL );
	}

	return succeeded;
}
