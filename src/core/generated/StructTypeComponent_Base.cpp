/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "StructTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.StructTypeComponent provides an interface named 'type', of type co.StructType ------ //

co::InterfaceType* StructTypeComponent_co_StructType::getInterfaceType()
{
	return co::typeOf<co::StructType>::get();
}

const std::string& StructTypeComponent_co_StructType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ StructTypeComponent_Base ------ //

StructTypeComponent_Base::StructTypeComponent_Base()
{
	// empty
}

StructTypeComponent_Base::~StructTypeComponent_Base()
{
	// empty
}

co::Component* StructTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void StructTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void StructTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* StructTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.StructTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* StructTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::StructType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void StructTypeComponent_Base::bindInterface( co::InterfaceInfo* receptacle, co::Interface* instance )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
