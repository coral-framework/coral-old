/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Type_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.Type provides an interface named 'type', of type co.IType ------ //

co::IInterfaceType* Type_co_IType::getInterfaceType()
{
	return co::typeOf<co::IType>::get();
}

const std::string& Type_co_IType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ Type_Base ------ //

Type_Base::Type_Base()
{
	// empty
}

Type_Base::~Type_Base()
{
	// empty
}

co::IComponent* Type_Base::getInterfaceOwner()
{
	return this;
}

void Type_Base::componentRetain()
{
	incrementRefCount();
}

void Type_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* Type_Base::getComponentType()
{
	co::IType* type = co::getType( "co.Type" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* Type_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Type_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
