/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ExceptionType_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.ExceptionType provides an interface named 'type', of type co.IException ------ //

co::IInterface* ExceptionType_co_IException::getInterfaceType()
{
	return co::typeOf<co::IException>::get();
}

const std::string& ExceptionType_co_IException::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ExceptionType_Base ------ //

ExceptionType_Base::ExceptionType_Base()
{
	// empty
}

ExceptionType_Base::~ExceptionType_Base()
{
	// empty
}

co::IObject* ExceptionType_Base::getInterfaceOwner()
{
	return this;
}

void ExceptionType_Base::componentRetain()
{
	incrementRefCount();
}

void ExceptionType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* ExceptionType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ExceptionType" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ExceptionType_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IException>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void ExceptionType_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
