/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodBuilder_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.MethodBuilder provides an interface named 'methodBuilder', of type co.IMethodBuilder ------ //

co::IInterfaceType* MethodBuilder_co_IMethodBuilder::getInterfaceType()
{
	return co::typeOf<co::IMethodBuilder>::get();
}

const std::string& MethodBuilder_co_IMethodBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "methodBuilder" );
	return s_interfaceName;
}

// ------ MethodBuilder_Base ------ //

MethodBuilder_Base::MethodBuilder_Base()
{
	// empty
}

MethodBuilder_Base::~MethodBuilder_Base()
{
	// empty
}

co::IComponent* MethodBuilder_Base::getInterfaceOwner()
{
	return this;
}

void MethodBuilder_Base::componentRetain()
{
	incrementRefCount();
}

void MethodBuilder_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* MethodBuilder_Base::getComponentType()
{
	co::IType* type = co::getType( "co.MethodBuilder" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* MethodBuilder_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IMethodBuilder>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void MethodBuilder_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
