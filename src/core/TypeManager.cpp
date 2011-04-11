/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeManager.h"
#include "Type.h"
#include "Coral.h"
#include "Namespace.h"
#include "ArrayType.h"
#include "TypeLoader.h"
#include "Interface.h"
#include "tools/StringTokenizer.h"
#include <co/CSLError.h>
#include <co/TypeLoadException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

namespace co {

static const std::string sg_emptyString;

TypeManager::TypeManager()
{
	_rootNS = new Namespace;
	_docParsing = true;
}

TypeManager::~TypeManager()
{
	// empty
}

void TypeManager::initialize()
{
	defineBuiltInTypes();
}

void TypeManager::addDocumentation( const std::string& typeOrMemberName, const std::string& text )
{
	DocMap::iterator it = _docMap.find( typeOrMemberName );
	if( it != _docMap.end() )
	{
		std::string& str = it->second;
		str.reserve( str.length() + 1 + text.length() );
		str.push_back( '\n' );
		str.append( text );
	}
	else
	{
		_docMap.insert( DocMap::value_type( typeOrMemberName, text ) );
	}
}

void TypeManager::addCppBlock( const std::string& interfaceName, const std::string& text )
{
	CppBlockMap::iterator it = _cppBlockMap.find( interfaceName );
	if( it != _cppBlockMap.end() )
		it->second.append( text );
	else
		_cppBlockMap.insert( CppBlockMap::value_type( interfaceName, text) );
}

const std::string& TypeManager::getCppBlock( const std::string& interfaceName )
{
	CppBlockMap::iterator it = _cppBlockMap.find( interfaceName );
	if( it == _cppBlockMap.end() )
		return sg_emptyString;

	return it->second;
}

INamespace* TypeManager::getRootNS()
{
	return _rootNS.get();
}

bool TypeManager::getDocumentationParsing()
{
	return _docParsing;
}

void TypeManager::setDocumentationParsing( bool documentationParsing )
{
	_docParsing = documentationParsing;
}

IType* TypeManager::findType( const std::string& fullName )
{
	INamespace* ns = _rootNS.get();
	assert( ns );

	StringTokenizer st( fullName, "." );
	st.nextToken();
	std::string currentToken = st.getToken();
	while( st.nextToken() )
	{
		ns = ns->getChildNamespace( currentToken );
		if( !ns )
			return NULL;

		currentToken = st.getToken();
	}

	return ns->getType( currentToken );
}

INamespace* TypeManager::findNamespace( const std::string& fullName )
{
	INamespace* ns = _rootNS.get();
	assert( ns );

	StringTokenizer st( fullName, "." );
	while( st.nextToken() )
	{
		ns = ns->getChildNamespace( st.getToken() );
		if( !ns )
			return NULL;
	}

	return ns;
}

IType* TypeManager::getType( const std::string& typeName )
{
	if( typeName.empty() )
		CORAL_THROW( IllegalArgumentException, "empty type name" );

	// is typeName an array?
	if( typeName[typeName.length() - 1] == ']' )
	{
		// get the elementType by removing the "[]" suffix
		IType* elementType = getType( std::string( typeName, 0, typeName.length() - 2 ) );
		return getArrayOf( elementType );
	}

	IType* type = findType( typeName );
	if( type )
		return type;

	return loadTypeOrThrow( typeName );
}

IArray* TypeManager::getArrayOf( IType* elementType )
{
	if( !elementType )
		CORAL_THROW( IllegalArgumentException, "null element type" );

	const std::string& elementTypeName = elementType->getName();

	std::string arrayName;
	arrayName.reserve( elementTypeName.length() + 2 );
	arrayName.append( elementTypeName );
	arrayName.append( "[]" );

	Namespace* ns = static_cast<Namespace*>( elementType->getNamespace() );

	// try to locate an existing array of this type
	IType* existingArrayType = ns->getType( arrayName );
	if( existingArrayType )
	{
		assert( existingArrayType->getKind() == TK_ARRAY );
		return static_cast<IArray*>( existingArrayType );
	}

	// otherwise, try to create it
	TypeKind kind = elementType->getKind();
	if( kind == TK_ARRAY )
		CORAL_THROW( IllegalArgumentException, "arrays of arrays are illegal" );

	if( kind == TK_EXCEPTION || kind == TK_COMPONENT )
		CORAL_THROW( IllegalArgumentException, "arrays of " <<
					( kind == TK_EXCEPTION ? "exception" : "component" ) << "s are illegal" );

	RefPtr<ArrayType> arrayType = new ArrayType;
	arrayType->setType( ns, elementType->getName() + "[]", TK_ARRAY );
	arrayType->setElementType( elementType );

	ns->addType( arrayType.get() );

	return arrayType.get();
}

IType* TypeManager::loadType( const std::string& typeName, std::vector<CSLError>& errorStack )
{
	IType* type = findType( typeName );
	if( type )
		return type;

	TypeLoader loader( typeName, getPaths(), this );

	type = loader.loadType();
	if( !type )
	{
		const csl::Error* currentError = loader.getError();
		while( currentError )
		{
			errorStack.push_back( CSLError() );

			CSLError& cslError = errorStack.back();
			cslError.filename = currentError->getFileName();
			cslError.message = currentError->getMessage();
			cslError.line = currentError->getLine();

			currentError = currentError->getInnerError();
		}
	}

	return type;
}

IType* TypeManager::loadTypeOrThrow( const std::string& fullName )
{
	TypeLoader loader( fullName, getPaths(), this );

	IType* type = loader.loadType();
	if( !type )
		CORAL_THROW( TypeLoadException, "could not load type '" << fullName << "':\n" << *loader.getError() );

	return type;
}

void TypeManager::definePrimitiveType( Namespace* ns, const std::string& name, TypeKind kind )
{
	RefPtr<Type> type = new Type;
	type->setType( ns, name, kind );
	ns->addType( type.get() );
}

void TypeManager::defineBuiltInTypes()
{
	Namespace* rootNS = _rootNS.get();

	// register all primitive types in the root namespace:
	for( unsigned int i = TK_ANY; i <= TK_STRING; ++i )
		definePrimitiveType( rootNS, TK_STRINGS[i], static_cast<TypeKind>( i ) );

	// pre-load the 'co.IService' type and all its dependencies
	INamespace* coNS = rootNS->defineChildNamespace( "co" );
	assert( coNS );
	Namespace* castNS = static_cast<Namespace*>( coNS );

	RefPtr<Interface> serviceType = new Interface;
	serviceType->setType( coNS, "IService", TK_INTERFACE );

	castNS->addType( serviceType.get() );

	loadTypeOrThrow( "co.IService" );
}

const std::string& TypeManager::getDocumentation( const std::string& typeOrMemberName )
{
	DocMap::iterator it = _docMap.find( typeOrMemberName );
	if( it == _docMap.end() )
		return sg_emptyString;

	return it->second;
}

CORAL_EXPORT_COMPONENT( TypeManager, TypeManager );

} // namespace co
