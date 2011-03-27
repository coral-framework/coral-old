/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Method_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.Method provides an interface named 'methodInfo', of type co.IMethod ------ //

co::IInterface* Method_co_IMethod::getInterfaceType()
{
	return co::typeOf<co::IMethod>::get();
}

const std::string& Method_co_IMethod::getInterfaceName()
{
	static const std::string s_interfaceName( "methodInfo" );
	return s_interfaceName;
}

// ------ Method_Base ------ //

Method_Base::Method_Base()
{
	// empty
}

Method_Base::~Method_Base()
{
	// empty
}

co::IObject* Method_Base::getInterfaceOwner()
{
	return this;
}

void Method_Base::componentRetain()
{
	incrementRefCount();
}

void Method_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* Method_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Method" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* Method_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IMethod>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Method_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
