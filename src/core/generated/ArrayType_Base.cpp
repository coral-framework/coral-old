/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ArrayType_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.ArrayType provides an interface named 'type', of type co.IArray ------ //

co::IInterface* ArrayType_co_IArray::getInterfaceType()
{
	return co::typeOf<co::IArray>::get();
}

const std::string& ArrayType_co_IArray::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ArrayType_Base ------ //

ArrayType_Base::ArrayType_Base()
{
	// empty
}

ArrayType_Base::~ArrayType_Base()
{
	// empty
}

co::IObject* ArrayType_Base::getInterfaceOwner()
{
	return this;
}

void ArrayType_Base::componentRetain()
{
	incrementRefCount();
}

void ArrayType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* ArrayType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ArrayType" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* ArrayType_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::IArray>( this ); break;
	default:	raiseUnexpectedPortIndex();
	}
	return res;
}

void ArrayType_Base::setReceptacle( co::IPort* receptacle, co::IService* service )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedPortIndex();
	CORAL_UNUSED( service );
}

} // namespace co
