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
#include <co/NoSuchPortException.h>
#include <co/IllegalCastException.h>
#include <sstream>

namespace co {

ComponentBase::~ComponentBase()
{
	if( _refCount != 0 )
		debug( Dbg_Fatal, "Deleting component %p with a reference count of %i.", this, _refCount );
}

IInterface* ComponentBase::getInterface()
{
	return typeOf<IObject>::get();
}

IPort* ComponentBase::getFacet()
{
	IPort* facet = static_cast<IPort*>( typeOf<IObject>::get()->getMember( "object" ) );
	assert( facet );
	return facet;
}

void ComponentBase::checkValidPort( IPort* port )
{
	if( !port )
		throw NoSuchPortException( "illegal null port" );

	IComponent* myType = getComponent();
	ICompositeType* owner = port->getOwner();
	if( owner != static_cast<ICompositeType*>( myType ) )
		CORAL_THROW( NoSuchPortException, "port '" << port->getName() << "' belongs to "
			<< owner->getFullName() << ", not to " << myType->getFullName() );

	assert( port->getIndex() < myType->getPorts().getSize() );
}

void ComponentBase::checkValidReceptacle( IPort* receptacle )
{
	checkValidPort( receptacle );
	if( receptacle->getIsFacet() )
		throw NoSuchPortException( "receptacle expected, got facet" );
}

void ComponentBase::raiseUnexpectedPortIndex()
{
	assert( false );
	throw NoSuchPortException( "unexpected invalid port index" );
}

void ComponentBase::raiseIncompatibleService( IInterface* expectedType, IService* service )
{
	CORAL_THROW( IllegalCastException, "incompatible service (" << expectedType->getFullName()
					<< " expected, got " << service->getInterface()->getFullName() << ")" );
}

IComponent* ComponentBase::getOrCreateInternalComponent(
	const char* componentName, const char* interfaceName, const char* facetName )
{
	std::string fullTypeName( componentName );

	// get the IComponent if it already exists
	ITypeManager* tm = getSystem()->getTypes();
	IType* type = tm->findType( fullTypeName );
	if( type )
	{
		assert( type->getKind() == TK_COMPONENT );
		return static_cast<IComponent*>( type );
	}

	// create the IComponent if it's not defined
	IType* itfType = getType( interfaceName );
	assert( itfType->getKind() == TK_INTERFACE );

	RefPtr<ITypeTransaction> transaction =
			newInstance( "co.TypeTransaction" )->getService<ITypeTransaction>();

	size_t lastDotPos = fullTypeName.rfind( '.' );
	assert( lastDotPos != std::string::npos ); // componentName must be specified with a namespace

	std::string namespaceName( fullTypeName.substr( 0, lastDotPos ) );
	std::string localTypeName( fullTypeName.substr( lastDotPos + 1 ) );

	INamespace* ns = tm->findNamespace( namespaceName );
	assert( ns ); // the namespace should have been created before

	RefPtr<ITypeBuilder> tb = ns->defineType( localTypeName, TK_COMPONENT, transaction.get() );
	tb->definePort( facetName, static_cast<IInterface*>( itfType ), true );

	try
	{
		transaction->commit();
	}
	catch( std::exception& )
	{
		transaction->rollback();
		throw;
	}

	type = tb->createType();
	assert( type && type->getKind() == TK_COMPONENT );

	// set the IComponent with a dummy reflector
	IComponent* component = static_cast<IComponent*>( type );
	component->setReflector( new BasicReflector( component ) );

	return component;
}

} // namespace 'co'
