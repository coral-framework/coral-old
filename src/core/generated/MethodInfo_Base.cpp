/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodInfo_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.MethodInfo provides an interface named 'methodInfo', of type co.IMethodInfo ------ //

co::IInterfaceType* MethodInfo_co_IMethodInfo::getInterfaceType()
{
	return co::typeOf<co::IMethodInfo>::get();
}

const std::string& MethodInfo_co_IMethodInfo::getInterfaceName()
{
	static const std::string s_interfaceName( "methodInfo" );
	return s_interfaceName;
}

// ------ MethodInfo_Base ------ //

MethodInfo_Base::MethodInfo_Base()
{
	// empty
}

MethodInfo_Base::~MethodInfo_Base()
{
	// empty
}

co::IComponent* MethodInfo_Base::getInterfaceOwner()
{
	return this;
}

void MethodInfo_Base::componentRetain()
{
	incrementRefCount();
}

void MethodInfo_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* MethodInfo_Base::getComponentType()
{
	co::IType* type = co::getType( "co.MethodInfo" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* MethodInfo_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IMethodInfo>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void MethodInfo_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
