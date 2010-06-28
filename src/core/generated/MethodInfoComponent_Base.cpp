/*******************************************************************************
** Base class generated for component 'co.MethodInfoComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "MethodInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.MethodInfoComponent provides an interface named 'methodInfo', of type co.MethodInfo ------ //

co::InterfaceType* co::MethodInfoComponent_co_MethodInfo::getInterfaceType()
{
	return co::typeOf<co::MethodInfo>::get();
}

const std::string& co::MethodInfoComponent_co_MethodInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "methodInfo" );
	return s_interfaceName;
}

// ------ MethodInfoComponent_Base ------ //

co::MethodInfoComponent_Base::MethodInfoComponent_Base()
{
	// empty
}

co::MethodInfoComponent_Base::~MethodInfoComponent_Base()
{
	// empty
}

co::Component* co::MethodInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::MethodInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::MethodInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::MethodInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.MethodInfoComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::MethodInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::MethodInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
