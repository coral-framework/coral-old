/*******************************************************************************
** Base class generated for component 'co.MethodBuilderComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "MethodBuilderComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.MethodBuilderComponent provides an interface named 'methodBuilder', of type co.MethodBuilder ------ //

co::InterfaceType* MethodBuilderComponent_co_MethodBuilder::getInterfaceType()
{
	return co::typeOf<co::MethodBuilder>::get();
}

const std::string& MethodBuilderComponent_co_MethodBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "methodBuilder" );
	return s_interfaceName;
}

// ------ MethodBuilderComponent_Base ------ //

MethodBuilderComponent_Base::MethodBuilderComponent_Base()
{
	// empty
}

MethodBuilderComponent_Base::~MethodBuilderComponent_Base()
{
	// empty
}

co::Component* MethodBuilderComponent_Base::getInterfaceOwner()
{
	return this;
}

void MethodBuilderComponent_Base::componentRetain()
{
	incrementRefCount();
}

void MethodBuilderComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* MethodBuilderComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.MethodBuilderComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* MethodBuilderComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void MethodBuilderComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
