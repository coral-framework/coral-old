/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "InterfaceType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.InterfaceType provides an interface named 'type', of type co.IInterfaceType ------ //

co::IInterfaceType* InterfaceType_co_IInterfaceType::getInterfaceType()
{
	return co::typeOf<co::IInterfaceType>::get();
}

const std::string& InterfaceType_co_IInterfaceType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ InterfaceType_Base ------ //

InterfaceType_Base::InterfaceType_Base()
{
	// empty
}

InterfaceType_Base::~InterfaceType_Base()
{
	// empty
}

co::IComponent* InterfaceType_Base::getInterfaceOwner()
{
	return this;
}

void InterfaceType_Base::componentRetain()
{
	incrementRefCount();
}

void InterfaceType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* InterfaceType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.InterfaceType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* InterfaceType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IInterfaceType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void InterfaceType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
