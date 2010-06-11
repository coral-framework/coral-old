/*******************************************************************************
** Base class generated for component 'co.NativeClassTypeComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "NativeClassTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.NativeClassTypeComponent provides an interface named 'type', of type co.NativeClassType ------ //

co::InterfaceType* co::NativeClassTypeComponent_co_NativeClassType::getInterfaceType()
{
	return co::typeOf<co::NativeClassType>::get();
}

const std::string& co::NativeClassTypeComponent_co_NativeClassType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ NativeClassTypeComponent_Base ------ //

co::NativeClassTypeComponent_Base::NativeClassTypeComponent_Base()
{
	// empty
}

co::NativeClassTypeComponent_Base::~NativeClassTypeComponent_Base()
{
	// empty
}

co::Component* co::NativeClassTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::NativeClassTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::NativeClassTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::NativeClassTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.NativeClassTypeComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::NativeClassTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::NativeClassType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::NativeClassTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
