/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeManager_Base.h"
#include <co/Coral.h>
#include <co/IComponent.h>
#include <co/IPort.h>
#include <co/IInterface.h>

namespace co {

// ------ co.TypeManager provides an interface named 'typeManager', of type co.ITypeManager ------ //

co::IInterface* TypeManager_co_ITypeManager::getInterfaceType()
{
	return co::typeOf<co::ITypeManager>::get();
}

const std::string& TypeManager_co_ITypeManager::getInterfaceName()
{
	static const std::string s_interfaceName( "typeManager" );
	return s_interfaceName;
}

// ------ TypeManager_Base ------ //

TypeManager_Base::TypeManager_Base()
{
	// empty
}

TypeManager_Base::~TypeManager_Base()
{
	// empty
}

co::IObject* TypeManager_Base::getInterfaceOwner()
{
	return this;
}

void TypeManager_Base::componentRetain()
{
	incrementRefCount();
}

void TypeManager_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponent* TypeManager_Base::getComponentType()
{
	co::IType* type = co::getType( "co.TypeManager" );
	assert( dynamic_cast<co::IComponent*>( type ) );
	return static_cast<co::IComponent*>( type );
}

co::IService* TypeManager_Base::getInterface( co::IPort* port )
{
	checkValidPort( port );
	co::IService* res = NULL;
	switch( port->getIndex() )
	{
	case 0:		res = co::disambiguate<co::IService, co::ITypeManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeManager_Base::setReceptacle( co::IPort* receptacle, co::IService* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
