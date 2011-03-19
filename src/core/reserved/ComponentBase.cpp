/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ComponentBase.h"
#include "../BasicReflector.h"
#include <co/Coral.h>
#include <co/RefPtr.h>
#include <co/ISystem.h>
#include <co/INamespace.h>
#include <co/ITypeBuilder.h>
#include <co/ITypeManager.h>
#include <co/ICompoundType.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>
#include <co/ITypeCreationTransaction.h>
#include <co/NoSuchInterfaceException.h>
#include <sstream>
#include <iostream>

namespace co {

ComponentBase::~ComponentBase()
{
	if( _refCount != 0 )
		debug( Dbg_Fatal, "Deleting component %p with a reference count of %i.", this, _refCount );
}

IInterfaceType* ComponentBase::getInterfaceType()
{
	return typeOf<IComponent>::get();
}

const std::string& ComponentBase::getInterfaceName()
{
	static const std::string s_interfaceName( "component" );
	return s_interfaceName;
}

void ComponentBase::checkValidInterface( IInterfaceInfo* itfInfo )
{
	if( !itfInfo )
		throw NoSuchInterfaceException( "illegal null interface info" );

	IComponentType* myType = getComponentType();
	ICompoundType* owner = itfInfo->getOwner();
	if( owner != static_cast<ICompoundType*>( myType ) )
		CORAL_THROW( NoSuchInterfaceException, "interface '" << itfInfo->getName() << "' belongs to "
			<< owner->getFullName() << ", not to " << myType->getFullName() );

	assert( itfInfo->getIndex() < myType->getInterfaces().getSize() );
}

void ComponentBase::checkValidReceptacle( IInterfaceInfo* receptacle )
{
	checkValidInterface( receptacle );
	if( receptacle->getIsFacet() )
		throw NoSuchInterfaceException( "receptacle expected, got facet" );
}

void ComponentBase::raiseUnexpectedInterfaceIndex()
{
	assert( false );
	throw NoSuchInterfaceException( "unexpected invalid interface index" );
}

void ComponentBase::raiseIncompatibleInterface( IInterfaceType* expectedType, Interface* ptr )
{
	CORAL_THROW( IllegalArgumentException, "incompatible interface types (" << expectedType->getFullName()
					<< " expected, got " << ptr->getInterfaceType()->getFullName() << ")" );
}

IComponentType* ComponentBase::getOrCreateSimpleInternalComponentType( const char* componentTypeName,
																	  const char* interfaceTypeName,
																	  const char* interfaceName )
{
	std::string fullTypeName( componentTypeName );

	// get the IComponentType if it already exists
	ITypeManager* tm = getSystem()->getTypes();
	IType* type = tm->findType( fullTypeName );
	if( type )
	{
		assert( dynamic_cast<IComponentType*>( type ) );
		return dynamic_cast<IComponentType*>( type );
	}

	// create the IComponentType if it's not defined
	IInterfaceType* interfaceType = dynamic_cast<IInterfaceType*>( getType( interfaceTypeName ) );
	assert( interfaceType );

	RefPtr<ITypeCreationTransaction> tct =
			newInstance( "co.TypeCreationTransaction" )->getFacet<ITypeCreationTransaction>();

	size_t lastDotPos = fullTypeName.rfind( '.' );
	assert( lastDotPos != std::string::npos ); // componentTypeName must be specified with a namespace

	std::string namespaceName( fullTypeName.substr( 0, lastDotPos ) );
	std::string localTypeName( fullTypeName.substr( lastDotPos + 1 ) );

	INamespace* ns = tm->findNamespace( namespaceName );
	assert( ns ); // the namespace should have been created before

	RefPtr<ITypeBuilder> tb = ns->defineType( localTypeName, TK_COMPONENT, tct.get() );
	tb->defineInterface( interfaceName, interfaceType, true );

	try
	{
		tct->commit();
	}
	catch( std::exception& )
	{
		tct->rollback();
		throw;
	}

	IComponentType* componentType = dynamic_cast<IComponentType*>( tb->createType() );
	assert( componentType );

	// set the IComponentType with a dummy reflector
	componentType->setReflector( new BasicReflector( componentType ) );

	return componentType;
}

} // namespace 'co'
