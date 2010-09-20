/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "TypeBuilderComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.TypeBuilderComponent provides an interface named 'typeBuilder', of type co.TypeBuilder ------ //

co::InterfaceType* TypeBuilderComponent_co_TypeBuilder::getInterfaceType()
{
	return co::typeOf<co::TypeBuilder>::get();
}

const std::string& TypeBuilderComponent_co_TypeBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "typeBuilder" );
	return s_interfaceName;
}

// ------ TypeBuilderComponent_Base ------ //

TypeBuilderComponent_Base::TypeBuilderComponent_Base()
{
	// empty
}

TypeBuilderComponent_Base::~TypeBuilderComponent_Base()
{
	// empty
}

co::Component* TypeBuilderComponent_Base::getInterfaceOwner()
{
	return this;
}

void TypeBuilderComponent_Base::componentRetain()
{
	incrementRefCount();
}

void TypeBuilderComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* TypeBuilderComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeBuilderComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* TypeBuilderComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::TypeBuilder>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeBuilderComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
