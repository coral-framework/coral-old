/*******************************************************************************
** Base class generated for component 'co.ParameterInfoComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ParameterInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.ParameterInfoComponent provides an interface named 'parameterInfo', of type co.ParameterInfo ------ //

co::InterfaceType* ParameterInfoComponent_co_ParameterInfo::getInterfaceType()
{
	return co::typeOf<co::ParameterInfo>::get();
}

const std::string& ParameterInfoComponent_co_ParameterInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "parameterInfo" );
	return s_interfaceName;
}

// ------ ParameterInfoComponent_Base ------ //

ParameterInfoComponent_Base::ParameterInfoComponent_Base()
{
	// empty
}

ParameterInfoComponent_Base::~ParameterInfoComponent_Base()
{
	// empty
}

co::Component* ParameterInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void ParameterInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void ParameterInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* ParameterInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ParameterInfoComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* ParameterInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ParameterInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ParameterInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
