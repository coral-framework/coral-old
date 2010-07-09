/*******************************************************************************
** Base class generated for component 'co.NativeClassTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "NativeClassTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.NativeClassTypeComponent provides an interface named 'type', of type co.NativeClassType ------ //

co::InterfaceType* NativeClassTypeComponent_co_NativeClassType::getInterfaceType()
{
	return co::typeOf<co::NativeClassType>::get();
}

const std::string& NativeClassTypeComponent_co_NativeClassType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ NativeClassTypeComponent_Base ------ //

NativeClassTypeComponent_Base::NativeClassTypeComponent_Base()
{
	// empty
}

NativeClassTypeComponent_Base::~NativeClassTypeComponent_Base()
{
	// empty
}

co::Component* NativeClassTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void NativeClassTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void NativeClassTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* NativeClassTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.NativeClassTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* NativeClassTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void NativeClassTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
