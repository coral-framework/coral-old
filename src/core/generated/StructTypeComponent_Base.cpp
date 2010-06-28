/*******************************************************************************
** Base class generated for component 'co.StructTypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "StructTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.StructTypeComponent provides an interface named 'type', of type co.StructType ------ //

co::InterfaceType* co::StructTypeComponent_co_StructType::getInterfaceType()
{
	return co::typeOf<co::StructType>::get();
}

const std::string& co::StructTypeComponent_co_StructType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ StructTypeComponent_Base ------ //

co::StructTypeComponent_Base::StructTypeComponent_Base()
{
	// empty
}

co::StructTypeComponent_Base::~StructTypeComponent_Base()
{
	// empty
}

co::Component* co::StructTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::StructTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::StructTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::StructTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.StructTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::StructTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::StructTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
