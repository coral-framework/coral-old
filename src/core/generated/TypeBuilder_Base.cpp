/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeBuilder_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.TypeBuilder provides an interface named 'typeBuilder', of type co.ITypeBuilder ------ //

co::IInterfaceType* TypeBuilder_co_ITypeBuilder::getInterfaceType()
{
	return co::typeOf<co::ITypeBuilder>::get();
}

const std::string& TypeBuilder_co_ITypeBuilder::getInterfaceName()
{
	static const std::string s_interfaceName( "typeBuilder" );
	return s_interfaceName;
}

// ------ TypeBuilder_Base ------ //

TypeBuilder_Base::TypeBuilder_Base()
{
	// empty
}

TypeBuilder_Base::~TypeBuilder_Base()
{
	// empty
}

co::IComponent* TypeBuilder_Base::getInterfaceOwner()
{
	return this;
}

void TypeBuilder_Base::componentRetain()
{
	incrementRefCount();
}

void TypeBuilder_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* TypeBuilder_Base::getComponentType()
{
	co::IType* type = co::getType( "co.TypeBuilder" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* TypeBuilder_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ITypeBuilder>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeBuilder_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
