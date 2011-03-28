/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodBuilder_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.MethodBuilder provides an interface named 'methodBuilder', of type co.IMethodBuilder ------ //

co::IInterface* MethodBuilder_co_IMethodBuilder::getInterfaceType()
{
	return co::typeOf<co::IMethodBuilder>::get();
}

const std::string& MethodBuilder_co_IMethodBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "methodBuilder" );
	return s_interfaceName;
}

// ------ MethodBuilder_Base ------ //

MethodBuilder_Base::MethodBuilder_Base()
{
	// empty
}

MethodBuilder_Base::~MethodBuilder_Base()
{
	// empty
}

co::IObject* MethodBuilder_Base::getInterfaceOwner()
{
	return this;
}

void MethodBuilder_Base::componentRetain()
{
	incrementRefCount();
}

void MethodBuilder_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* MethodBuilder_Base::getComponentType()
{
	co::IType* type = co::getType( "co.MethodBuilder" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* MethodBuilder_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IMethodBuilder>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void MethodBuilder_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
