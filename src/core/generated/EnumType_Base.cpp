/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "EnumType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.EnumType provides an interface named 'type', of type co.IEnumType ------ //

co::IInterfaceType* EnumType_co_IEnumType::getInterfaceType()
{
	return co::typeOf<co::IEnumType>::get();
}

const std::string& EnumType_co_IEnumType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ EnumType_Base ------ //

EnumType_Base::EnumType_Base()
{
	// empty
}

EnumType_Base::~EnumType_Base()
{
	// empty
}

co::IComponent* EnumType_Base::getInterfaceOwner()
{
	return this;
}

void EnumType_Base::componentRetain()
{
	incrementRefCount();
}

void EnumType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* EnumType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.EnumType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* EnumType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IEnumType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void EnumType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
