/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "AttributeInfo_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.AttributeInfo provides an interface named 'attributeInfo', of type co.IAttributeInfo ------ //

co::IInterfaceType* AttributeInfo_co_IAttributeInfo::getInterfaceType()
{
	return co::typeOf<co::IAttributeInfo>::get();
}

const std::string& AttributeInfo_co_IAttributeInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "attributeInfo" );
	return s_interfaceName;
}

// ------ AttributeInfo_Base ------ //

AttributeInfo_Base::AttributeInfo_Base()
{
	// empty
}

AttributeInfo_Base::~AttributeInfo_Base()
{
	// empty
}

co::IComponent* AttributeInfo_Base::getInterfaceOwner()
{
	return this;
}

void AttributeInfo_Base::componentRetain()
{
	incrementRefCount();
}

void AttributeInfo_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* AttributeInfo_Base::getComponentType()
{
	co::IType* type = co::getType( "co.AttributeInfo" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* AttributeInfo_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IAttributeInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void AttributeInfo_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
