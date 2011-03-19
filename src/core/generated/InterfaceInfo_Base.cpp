/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "InterfaceInfo_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.InterfaceInfo provides an interface named 'interfaceInfo', of type co.IInterfaceInfo ------ //

co::IInterfaceType* InterfaceInfo_co_IInterfaceInfo::getInterfaceType()
{
	return co::typeOf<co::IInterfaceInfo>::get();
}

const std::string& InterfaceInfo_co_IInterfaceInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "interfaceInfo" );
	return s_interfaceName;
}

// ------ InterfaceInfo_Base ------ //

InterfaceInfo_Base::InterfaceInfo_Base()
{
	// empty
}

InterfaceInfo_Base::~InterfaceInfo_Base()
{
	// empty
}

co::IComponent* InterfaceInfo_Base::getInterfaceOwner()
{
	return this;
}

void InterfaceInfo_Base::componentRetain()
{
	incrementRefCount();
}

void InterfaceInfo_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* InterfaceInfo_Base::getComponentType()
{
	co::IType* type = co::getType( "co.InterfaceInfo" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* InterfaceInfo_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IInterfaceInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void InterfaceInfo_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
