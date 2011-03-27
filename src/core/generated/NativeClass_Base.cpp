/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "NativeClass_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.NativeClass provides an interface named 'type', of type co.INativeClass ------ //

co::IInterface* NativeClass_co_INativeClass::getInterfaceType()
{
	return co::typeOf<co::INativeClass>::get();
}

const std::string& NativeClass_co_INativeClass::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ NativeClass_Base ------ //

NativeClass_Base::NativeClass_Base()
{
	// empty
}

NativeClass_Base::~NativeClass_Base()
{
	// empty
}

co::IObject* NativeClass_Base::getInterfaceOwner()
{
	return this;
}

void NativeClass_Base::componentRetain()
{
	incrementRefCount();
}

void NativeClass_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* NativeClass_Base::getComponentType()
{
	co::IType* type = co::getType( "co.NativeClass" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* NativeClass_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::INativeClass>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void NativeClass_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
