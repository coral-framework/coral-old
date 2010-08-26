/*******************************************************************************
** Base class generated for component 'co.ArrayTypeComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ArrayTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ArrayTypeComponent provides an interface named 'type', of type co.ArrayType ------ //

co::InterfaceType* ArrayTypeComponent_co_ArrayType::getInterfaceType()
{
	return co::typeOf<co::ArrayType>::get();
}

const std::string& ArrayTypeComponent_co_ArrayType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ArrayTypeComponent_Base ------ //

ArrayTypeComponent_Base::ArrayTypeComponent_Base()
{
	// empty
}

ArrayTypeComponent_Base::~ArrayTypeComponent_Base()
{
	// empty
}

co::Component* ArrayTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void ArrayTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ArrayTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ArrayTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ArrayTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ArrayTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void ArrayTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
