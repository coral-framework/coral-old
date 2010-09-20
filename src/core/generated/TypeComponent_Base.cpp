/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.TypeComponent provides an interface named 'type', of type co.Type ------ //

co::InterfaceType* TypeComponent_co_Type::getInterfaceType()
{
	return co::typeOf<co::Type>::get();
}

const std::string& TypeComponent_co_Type::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ TypeComponent_Base ------ //

TypeComponent_Base::TypeComponent_Base()
{
	// empty
}

TypeComponent_Base::~TypeComponent_Base()
{
	// empty
}

co::Component* TypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void TypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void TypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* TypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* TypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::Type>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
