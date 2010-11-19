/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.MethodInfoComponent provides an interface named 'methodInfo', of type co.MethodInfo ------ //

co::InterfaceType* MethodInfoComponent_co_MethodInfo::getInterfaceType()
{
	return co::typeOf<co::MethodInfo>::get();
}

const std::string& MethodInfoComponent_co_MethodInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "methodInfo" );
	return s_interfaceName;
}

// ------ MethodInfoComponent_Base ------ //

MethodInfoComponent_Base::MethodInfoComponent_Base()
{
	// empty
}

MethodInfoComponent_Base::~MethodInfoComponent_Base()
{
	// empty
}

co::Component* MethodInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void MethodInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void MethodInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* MethodInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.MethodInfoComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* MethodInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::MethodInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void MethodInfoComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
