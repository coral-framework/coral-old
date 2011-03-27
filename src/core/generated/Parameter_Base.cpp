/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Parameter_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Parameter provides an interface named 'parameter', of type co.IParameter ------ //

co::IInterface* Parameter_co_IParameter::getInterfaceType()
{
	return co::typeOf<co::IParameter>::get();
}

const std::string& Parameter_co_IParameter::getInterfaceName()
{
	static const std::string s_interfaceName( "parameter" );
	return s_interfaceName;
}

// ------ Parameter_Base ------ //

Parameter_Base::Parameter_Base()
{
	// empty
}

Parameter_Base::~Parameter_Base()
{
	// empty
}

co::IObject* Parameter_Base::getInterfaceOwner()
{
	return this;
}

void Parameter_Base::componentRetain()
{
	incrementRefCount();
}

void Parameter_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Parameter_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Parameter" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Parameter_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IParameter>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Parameter_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
