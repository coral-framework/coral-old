/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeCreationTransaction_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.TypeCreationTransaction provides an interface named 'transaction', of type co.ITypeCreationTransaction ------ //

co::IInterfaceType* TypeCreationTransaction_co_ITypeCreationTransaction::getInterfaceType()
{
	return co::typeOf<co::ITypeCreationTransaction>::get();
}

const std::string& TypeCreationTransaction_co_ITypeCreationTransaction::getInterfaceName()
{
	static const std::string s_interfaceName( "transaction" );
	return s_interfaceName;
}

// ------ TypeCreationTransaction_Base ------ //

TypeCreationTransaction_Base::TypeCreationTransaction_Base()
{
	// empty
}

TypeCreationTransaction_Base::~TypeCreationTransaction_Base()
{
	// empty
}

co::IComponent* TypeCreationTransaction_Base::getInterfaceOwner()
{
	return this;
}

void TypeCreationTransaction_Base::componentRetain()
{
	incrementRefCount();
}

void TypeCreationTransaction_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* TypeCreationTransaction_Base::getComponentType()
{
	co::IType* type = co::getType( "co.TypeCreationTransaction" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* TypeCreationTransaction_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ITypeCreationTransaction>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeCreationTransaction_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
