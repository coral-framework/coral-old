/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeManager.h"
#include "Type.h"
#include "Coral.h"
#include "ArrayType.h"
#include "TypeLoader.h"
#include "InterfaceType.h"
#include "tools/StringTokenizer.h"
#include <co/CSLError.h>
#include <co/TypeLoadException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

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
		it->second.append( text );
	else
		_docMap.insert( DocMap::value_type( typeOrMemberName, text ) );
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

co::Namespace* TypeManager::getRootNS()
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

co::Type* TypeManager::findType( const std::string& fullName )
{
	co::Namespace* ns = _rootNS.get();
	assert( ns );
	
	co::StringTokenizer st( fullName, "." );
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

co::Namespace* TypeManager::findNamespace( const std::string& fullName )
{
	co::Namespace* ns = _rootNS.get();
	assert( ns );

	co::StringTokenizer st( fullName, "." );
	while( st.nextToken() )
	{
		ns = ns->getChildNamespace( st.getToken() );
		if( !ns )
			return NULL;
	}

	return ns;
}

co::Type* TypeManager::getType( const std::string& typeName )
{
	if( typeName.empty() )
		CORAL_THROW( co::IllegalArgumentException, "empty type name" );

	// is typeName an array?
	if( typeName[typeName.length() - 1] == ']' )
	{
		// get the elementType by removing the "[]" suffix
		co::Type* elementType = getType( std::string( typeName, 0, typeName.length() - 2 ) );
		return getArrayOf( elementType );
	}

	co::Type* type = findType( typeName );
	if( type )
		return type;

	return loadTypeOrThrow( typeName );
}

co::ArrayType* TypeManager::getArrayOf( co::Type* elementType )
{
	if( !elementType )
		CORAL_THROW( co::IllegalArgumentException, "null element type" );

	const std::string& elementTypeName = elementType->getName();

	std::string arrayName;
	arrayName.reserve( elementTypeName.length() + 2 );
	arrayName.append( elementTypeName );
	arrayName.append( "[]" );

	Namespace* ns = static_cast<Namespace*>( elementType->getNamespace() );

	// try to locate an existing array of this type
	co::Type* existingArrayType = ns->getType( arrayName );
	if( existingArrayType )
	{
		assert( dynamic_cast<co::ArrayType*>( existingArrayType ) );
		return static_cast<co::ArrayType*>( existingArrayType );
	}

	// otherwise, try to create it
	co::TypeKind kind = elementType->getKind();
	if( kind == co::TK_ARRAY )
		CORAL_THROW( co::IllegalArgumentException, "arrays of arrays are illegal" );

	if( kind == co::TK_EXCEPTION || kind == co::TK_COMPONENT )
		CORAL_THROW( co::IllegalArgumentException, "arrays of " <<
					( kind == co::TK_EXCEPTION ? "exception" : "component" ) << "s are illegal" );

	co::RefPtr<ArrayType> arrayType = new ArrayType;
	arrayType->setType( ns, elementType->getName() + "[]", co::TK_ARRAY );
	arrayType->setElementType( elementType );

	ns->addType( arrayType.get() );

	return arrayType.get();
}

co::Type* TypeManager::loadType( const std::string& typeName, std::vector<co::CSLError>* errorStack )
{
	co::Type* type = findType( typeName );
	if( type )
		return type;

	TypeLoader loader( typeName, co::getPaths(), this );

	type = loader.loadType();
	if( !type && errorStack )
	{
		const csl::Error* currentError = loader.getError();
		while( currentError )
		{
			errorStack->push_back( co::CSLError() );

			co::CSLError& cslError = errorStack->back();
			cslError.filename = currentError->getFileName();
			cslError.message = currentError->getMessage();
			cslError.line = currentError->getLine();
			
			currentError = currentError->getInnerError();
		}
	}

	return type;
}

co::Type* TypeManager::loadTypeOrThrow( const std::string& fullName )
{
	TypeLoader loader( fullName, co::getPaths(), this );

	co::Type* type = loader.loadType();
	if( !type )
		CORAL_THROW( co::TypeLoadException, "could not load type '" << fullName << "':\n" << *loader.getError() );

	return type;
}

void TypeManager::definePrimitiveType( Namespace* ns, const std::string& name, co::TypeKind kind )
{
	co::RefPtr<Type> type = new Type;
	type->setType( ns, name, kind );
	ns->addType( type.get() );
}

void TypeManager::defineBuiltInTypes()
{
	Namespace* rootNS = _rootNS.get();

	// register all primitive types in the root namespace:
	for( unsigned int i = co::TK_ANY; i <= co::TK_STRING; ++i )
		definePrimitiveType( rootNS, co::TK_STRINGS[i], static_cast<co::TypeKind>( i ) );

	// pre-load the 'co.Interface' type and all its dependencies
	Namespace* coNS = dynamic_cast<Namespace*>( rootNS->defineChildNamespace( "co" ) );
	assert( coNS );

	co::RefPtr<InterfaceType> coInterfaceType = new InterfaceType;
	coInterfaceType->setType( coNS, "Interface", co::TK_INTERFACE );

	coNS->addType( coInterfaceType.get() );

	loadTypeOrThrow( "co.Interface" );
}

const std::string& TypeManager::getDocumentation( const std::string& typeOrMemberName )
{
	DocMap::iterator it = _docMap.find( typeOrMemberName );
	if( it == _docMap.end() )
		return sg_emptyString;

	return it->second;
}

CORAL_EXPORT_COMPONENT( TypeManager, TypeManagerComponent );
