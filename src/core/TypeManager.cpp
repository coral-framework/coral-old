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
#include "InterfaceType.h"
#include "tools/StringTokenizer.h"
#include <co/CSLError.h>
#include <co/TypeLoadException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

namespace co {

static const std::string sg_emptyString;

TypeManagerComponent::TypeManagerComponent()
{
	_rootNS = new NamespaceComponent;
	_docParsing = true;
}

TypeManagerComponent::~TypeManagerComponent()
{
	// empty
}

void TypeManagerComponent::initialize()
{
	defineBuiltInTypes();
}

void TypeManagerComponent::addDocumentation( const std::string& typeOrMemberName, const std::string& text )
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

void TypeManagerComponent::addCppBlock( const std::string& interfaceName, const std::string& text )
{
	CppBlockMap::iterator it = _cppBlockMap.find( interfaceName );
	if( it != _cppBlockMap.end() )
		it->second.append( text );
	else
		_cppBlockMap.insert( CppBlockMap::value_type( interfaceName, text) );
}

const std::string& TypeManagerComponent::getCppBlock( const std::string& interfaceName )
{
	CppBlockMap::iterator it = _cppBlockMap.find( interfaceName );
	if( it == _cppBlockMap.end() )
		return sg_emptyString;

	return it->second;
}

Namespace* TypeManagerComponent::getRootNS()
{
	return _rootNS.get();
}

bool TypeManagerComponent::getDocumentationParsing()
{
	return _docParsing;
}

void TypeManagerComponent::setDocumentationParsing( bool documentationParsing )
{
	_docParsing = documentationParsing;
}

Type* TypeManagerComponent::findType( const std::string& fullName )
{
	Namespace* ns = _rootNS.get();
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

Namespace* TypeManagerComponent::findNamespace( const std::string& fullName )
{
	Namespace* ns = _rootNS.get();
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

Type* TypeManagerComponent::getType( const std::string& typeName )
{
	if( typeName.empty() )
		CORAL_THROW( IllegalArgumentException, "empty type name" );

	// is typeName an array?
	if( typeName[typeName.length() - 1] == ']' )
	{
		// get the elementType by removing the "[]" suffix
		Type* elementType = getType( std::string( typeName, 0, typeName.length() - 2 ) );
		return getArrayOf( elementType );
	}

	Type* type = findType( typeName );
	if( type )
		return type;

	return loadTypeOrThrow( typeName );
}

ArrayType* TypeManagerComponent::getArrayOf( Type* elementType )
{
	if( !elementType )
		CORAL_THROW( IllegalArgumentException, "null element type" );

	const std::string& elementTypeName = elementType->getName();

	std::string arrayName;
	arrayName.reserve( elementTypeName.length() + 2 );
	arrayName.append( elementTypeName );
	arrayName.append( "[]" );

	assert( dynamic_cast<NamespaceComponent*>( elementType->getNamespace() ) );
	NamespaceComponent* ns = static_cast<NamespaceComponent*>( elementType->getNamespace() );

	// try to locate an existing array of this type
	Type* existingArrayType = ns->getType( arrayName );
	if( existingArrayType )
	{
		assert( dynamic_cast<ArrayType*>( existingArrayType ) );
		return static_cast<ArrayType*>( existingArrayType );
	}

	// otherwise, try to create it
	TypeKind kind = elementType->getKind();
	if( kind == TK_ARRAY )
		CORAL_THROW( IllegalArgumentException, "arrays of arrays are illegal" );

	if( kind == TK_EXCEPTION || kind == TK_COMPONENT )
		CORAL_THROW( IllegalArgumentException, "arrays of " <<
					( kind == TK_EXCEPTION ? "exception" : "component" ) << "s are illegal" );

	RefPtr<ArrayTypeComponent> arrayType = new ArrayTypeComponent;
	arrayType->setType( ns, elementType->getName() + "[]", TK_ARRAY );
	arrayType->setElementType( elementType );

	ns->addType( arrayType.get() );

	return arrayType.get();
}

Type* TypeManagerComponent::loadType( const std::string& typeName, std::vector<CSLError>& errorStack )
{
	Type* type = findType( typeName );
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

Type* TypeManagerComponent::loadTypeOrThrow( const std::string& fullName )
{
	TypeLoader loader( fullName, getPaths(), this );

	Type* type = loader.loadType();
	if( !type )
		CORAL_THROW( TypeLoadException, "could not load type '" << fullName << "':\n" << *loader.getError() );

	return type;
}

void TypeManagerComponent::definePrimitiveType( NamespaceComponent* ns, const std::string& name, TypeKind kind )
{
	RefPtr<TypeComponent> type = new TypeComponent;
	type->setType( ns, name, kind );
	ns->addType( type.get() );
}

void TypeManagerComponent::defineBuiltInTypes()
{
	NamespaceComponent* rootNS = _rootNS.get();

	// register all primitive types in the root namespace:
	for( unsigned int i = TK_ANY; i <= TK_STRING; ++i )
		definePrimitiveType( rootNS, TK_STRINGS[i], static_cast<TypeKind>( i ) );

	// pre-load the 'co.Interface' type and all its dependencies
	Namespace* coNS = rootNS->defineChildNamespace( "co" );
	assert( dynamic_cast<NamespaceComponent*>( coNS ) );
	NamespaceComponent* castNS = static_cast<NamespaceComponent*>( coNS );

	RefPtr<InterfaceTypeComponent> coInterfaceType = new InterfaceTypeComponent;
	coInterfaceType->setType( coNS, "Interface", TK_INTERFACE );

	castNS->addType( coInterfaceType.get() );

	loadTypeOrThrow( "co.Interface" );
}

const std::string& TypeManagerComponent::getDocumentation( const std::string& typeOrMemberName )
{
	DocMap::iterator it = _docMap.find( typeOrMemberName );
	if( it == _docMap.end() )
		return sg_emptyString;

	return it->second;
}

CORAL_EXPORT_COMPONENT( TypeManagerComponent, TypeManagerComponent );

} // namespace co
