/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "StructType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.StructType provides an interface named 'type', of type co.IStructType ------ //

co::IInterfaceType* StructType_co_IStructType::getInterfaceType()
{
	return co::typeOf<co::IStructType>::get();
}

const std::string& StructType_co_IStructType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ StructType_Base ------ //

StructType_Base::StructType_Base()
{
	// empty
}

StructType_Base::~StructType_Base()
{
	// empty
}

co::IComponent* StructType_Base::getInterfaceOwner()
{
	return this;
}

void StructType_Base::componentRetain()
{
	incrementRefCount();
}

void StructType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* StructType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.StructType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* StructType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IStructType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void StructType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
