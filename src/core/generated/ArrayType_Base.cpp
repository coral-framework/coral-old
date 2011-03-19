/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ArrayType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ArrayType provides an interface named 'type', of type co.IArrayType ------ //

co::IInterfaceType* ArrayType_co_IArrayType::getInterfaceType()
{
	return co::typeOf<co::IArrayType>::get();
}

const std::string& ArrayType_co_IArrayType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ArrayType_Base ------ //

ArrayType_Base::ArrayType_Base()
{
	// empty
}

ArrayType_Base::~ArrayType_Base()
{
	// empty
}

co::IComponent* ArrayType_Base::getInterfaceOwner()
{
	return this;
}

void ArrayType_Base::componentRetain()
{
	incrementRefCount();
}

void ArrayType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* ArrayType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ArrayType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ArrayType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IArrayType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ArrayType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
