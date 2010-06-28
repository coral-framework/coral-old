/*******************************************************************************
** Base class generated for component 'co.MethodBuilderComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "MethodBuilderComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.MethodBuilderComponent provides an interface named 'methodBuilder', of type co.MethodBuilder ------ //

co::InterfaceType* co::MethodBuilderComponent_co_MethodBuilder::getInterfaceType()
{
	return co::typeOf<co::MethodBuilder>::get();
}

const std::string& co::MethodBuilderComponent_co_MethodBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "methodBuilder" );
	return s_interfaceName;
}

// ------ MethodBuilderComponent_Base ------ //

co::MethodBuilderComponent_Base::MethodBuilderComponent_Base()
{
	// empty
}

co::MethodBuilderComponent_Base::~MethodBuilderComponent_Base()
{
	// empty
}

co::Component* co::MethodBuilderComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::MethodBuilderComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::MethodBuilderComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::MethodBuilderComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.MethodBuilderComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::MethodBuilderComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::MethodBuilder>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::MethodBuilderComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
