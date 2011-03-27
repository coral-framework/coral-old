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
#include <co/ICompositeType.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>
#include <co/ITypeTransaction.h>
#include <co/NoSuchInterfaceException.h>
#include <sstream>
#include <iostream>

namespace co {

ComponentBase::~ComponentBase()
{
	if( _refCount != 0 )
		debug( Dbg_Fatal, "Deleting component %p with a reference count of %i.", this, _refCount );
}

IInterface* ComponentBase::getInterfaceType()
{
	return typeOf<IObject>::get();
}

const std::string& ComponentBase::getInterfaceName()
{
	static const std::string s_interfaceName( "component" );
	return s_interfaceName;
}

void ComponentBase::checkValidPort( IPort* port )
{
	if( !port )
		throw NoSuchInterfaceException( "illegal null port" );

	IComponent* myType = getComponentType();
	ICompositeType* owner = port->getOwner();
	if( owner != static_cast<ICompositeType*>( myType ) )
		CORAL_THROW( NoSuchInterfaceException, "port '" << port->getName() << "' belongs to "
			<< owner->getFullName() << ", not to " << myType->getFullName() );

	assert( port->getIndex() < myType->getPorts().getSize() );
}

void ComponentBase::checkValidReceptacle( IPort* receptacle )
{
	checkValidPort( receptacle );
	if( receptacle->getIsFacet() )
		throw NoSuchInterfaceException( "receptacle expected, got facet" );
}

void ComponentBase::raiseUnexpectedInterfaceIndex()
{
	assert( false );
	throw NoSuchInterfaceException( "unexpected invalid interface index" );
}

void ComponentBase::raiseIncompatibleInterface( IInterface* expectedType, IService* ptr )
{
	CORAL_THROW( IllegalArgumentException, "incompatible interface types (" << expectedType->getFullName()
					<< " expected, got " << ptr->getInterfaceType()->getFullName() << ")" );
}

IComponent* ComponentBase::getOrCreateSimpleInternalComponentType( const char* componentTypeName,
																	  const char* interfaceTypeName,
																	  const char* interfaceName )
{
	std::string fullTypeName( componentTypeName );

	// get the IComponent if it already exists
	ITypeManager* tm = getSystem()->getTypes();
	IType* type = tm->findType( fullTypeName );
	if( type )
	{
		assert( dynamic_cast<IComponent*>( type ) );
		return dynamic_cast<IComponent*>( type );
	}

	// create the IComponent if it's not defined
	IInterface* interfaceType = dynamic_cast<IInterface*>( getType( interfaceTypeName ) );
	assert( interfaceType );

	RefPtr<ITypeTransaction> tct =
			newInstance( "co.TypeTransaction" )->getFacet<ITypeTransaction>();

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

	IComponent* componentType = dynamic_cast<IComponent*>( tb->createType() );
	assert( componentType );

	// set the IComponent with a dummy reflector
	componentType->setReflector( new BasicReflector( componentType ) );

	return componentType;
}

} // namespace 'co'
