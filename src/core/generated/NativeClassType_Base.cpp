/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "NativeClassType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.NativeClassType provides an interface named 'type', of type co.INativeClassType ------ //

co::IInterfaceType* NativeClassType_co_INativeClassType::getInterfaceType()
{
	return co::typeOf<co::INativeClassType>::get();
}

const std::string& NativeClassType_co_INativeClassType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ NativeClassType_Base ------ //

NativeClassType_Base::NativeClassType_Base()
{
	// empty
}

NativeClassType_Base::~NativeClassType_Base()
{
	// empty
}

co::IComponent* NativeClassType_Base::getInterfaceOwner()
{
	return this;
}

void NativeClassType_Base::componentRetain()
{
	incrementRefCount();
}

void NativeClassType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* NativeClassType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.NativeClassType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* NativeClassType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::INativeClassType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void NativeClassType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
