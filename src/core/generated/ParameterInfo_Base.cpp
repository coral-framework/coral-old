/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ParameterInfo_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ParameterInfo provides an interface named 'parameterInfo', of type co.IParameterInfo ------ //

co::IInterfaceType* ParameterInfo_co_IParameterInfo::getInterfaceType()
{
	return co::typeOf<co::IParameterInfo>::get();
}

const std::string& ParameterInfo_co_IParameterInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "parameterInfo" );
	return s_interfaceName;
}

// ------ ParameterInfo_Base ------ //

ParameterInfo_Base::ParameterInfo_Base()
{
	// empty
}

ParameterInfo_Base::~ParameterInfo_Base()
{
	// empty
}

co::IComponent* ParameterInfo_Base::getInterfaceOwner()
{
	return this;
}

void ParameterInfo_Base::componentRetain()
{
	incrementRefCount();
}

void ParameterInfo_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* ParameterInfo_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ParameterInfo" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ParameterInfo_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IParameterInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ParameterInfo_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
