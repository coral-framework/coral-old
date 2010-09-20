/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "InterfaceInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.InterfaceInfoComponent provides an interface named 'interfaceInfo', of type co.InterfaceInfo ------ //

co::InterfaceType* InterfaceInfoComponent_co_InterfaceInfo::getInterfaceType()
{
	return co::typeOf<co::InterfaceInfo>::get();
}

const std::string& InterfaceInfoComponent_co_InterfaceInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "interfaceInfo" );
	return s_interfaceName;
}

// ------ InterfaceInfoComponent_Base ------ //

InterfaceInfoComponent_Base::InterfaceInfoComponent_Base()
{
	// empty
}

InterfaceInfoComponent_Base::~InterfaceInfoComponent_Base()
{
	// empty
}

co::Component* InterfaceInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void InterfaceInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void InterfaceInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* InterfaceInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.InterfaceInfoComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* InterfaceInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::InterfaceInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void InterfaceInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
