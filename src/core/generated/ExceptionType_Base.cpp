/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "ExceptionType_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.ExceptionType provides an interface named 'type', of type co.IExceptionType ------ //

co::IInterfaceType* ExceptionType_co_IExceptionType::getInterfaceType()
{
	return co::typeOf<co::IExceptionType>::get();
}

const std::string& ExceptionType_co_IExceptionType::getInterfaceName()
{
	static const std::string s_interfaceName( "type" );
	return s_interfaceName;
}

// ------ ExceptionType_Base ------ //

ExceptionType_Base::ExceptionType_Base()
{
	// empty
}

ExceptionType_Base::~ExceptionType_Base()
{
	// empty
}

co::IComponent* ExceptionType_Base::getInterfaceOwner()
{
	return this;
}

void ExceptionType_Base::componentRetain()
{
	incrementRefCount();
}

void ExceptionType_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* ExceptionType_Base::getComponentType()
{
	co::IType* type = co::getType( "co.ExceptionType" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* ExceptionType_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::IExceptionType>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void ExceptionType_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
