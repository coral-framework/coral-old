/*******************************************************************************
** Base class generated for component 'co.ExceptionTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ExceptionTypeComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ExceptionTypeComponent provides an interface named 'type', of type co.ExceptionType ------ //

co::InterfaceType* ExceptionTypeComponent_co_ExceptionType::getInterfaceType()
{
	return co::typeOf<co::ExceptionType>::get();
}

const std::string& ExceptionTypeComponent_co_ExceptionType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ExceptionTypeComponent_Base ------ //

ExceptionTypeComponent_Base::ExceptionTypeComponent_Base()
{
	// empty
}

ExceptionTypeComponent_Base::~ExceptionTypeComponent_Base()
{
	// empty
}

co::Component* ExceptionTypeComponent_Base::getInterfaceOwner()
{
	return this;
}

void ExceptionTypeComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ExceptionTypeComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ExceptionTypeComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ExceptionTypeComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ExceptionTypeComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ExceptionType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ExceptionTypeComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
