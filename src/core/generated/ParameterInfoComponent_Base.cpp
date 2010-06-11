/*******************************************************************************
** Base class generated for component 'co.ParameterInfoComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "ParameterInfoComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>

// ------ co.ParameterInfoComponent provides an interface named 'parameterInfo', of type co.ParameterInfo ------ //

co::InterfaceType* co::ParameterInfoComponent_co_ParameterInfo::getInterfaceType()
{
	return co::typeOf<co::ParameterInfo>::get();
}

const std::string& co::ParameterInfoComponent_co_ParameterInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "parameterInfo" );
	return s_interfaceName;
}

// ------ ParameterInfoComponent_Base ------ //

co::ParameterInfoComponent_Base::ParameterInfoComponent_Base()
{
	// empty
}

co::ParameterInfoComponent_Base::~ParameterInfoComponent_Base()
{
	// empty
}

co::Component* co::ParameterInfoComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::ParameterInfoComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::ParameterInfoComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::ParameterInfoComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.ParameterInfoComponent" );
	co::ComponentType* componentType = dynamic_cast<co::ComponentType*>( type );
	assert( componentType );
	return componentType;
}

co::Interface* co::ParameterInfoComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::ParameterInfoComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
