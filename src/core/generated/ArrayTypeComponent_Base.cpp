/*******************************************************************************
** Base class generated for component 'co.ArrayTypeComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ArrayTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.ArrayTypeComponent provides an interface named 'type', of type co.ArrayType ------ //

co::InterfaceType* co::ArrayTypeComponent_co_ArrayType::getInterfaceType()
{
	return co::typeOf<co::ArrayType>::get();
}

const std::string& co::ArrayTypeComponent_co_ArrayType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ArrayTypeComponent_Base ------ //

co::ArrayTypeComponent_Base::ArrayTypeComponent_Base()
{
	// empty
}

co::ArrayTypeComponent_Base::~ArrayTypeComponent_Base()
{
	// empty
}

co::Component* co::ArrayTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::ArrayTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::ArrayTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::ArrayTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ArrayTypeComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::ArrayTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ArrayType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void co::ArrayTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
