/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "TypeManager_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace co {

// ------ co.TypeManager provides an interface named 'typeManager', of type co.ITypeManager ------ //

co::IInterfaceType* TypeManager_co_ITypeManager::getInterfaceType()
{
	return co::typeOf<co::ITypeManager>::get();
}

const std::string& TypeManager_co_ITypeManager::getInterfaceName()
{
	static const std::string s_interfaceName( "typeManager" );
	return s_interfaceName;
}

// ------ TypeManager_Base ------ //

TypeManager_Base::TypeManager_Base()
{
	// empty
}

TypeManager_Base::~TypeManager_Base()
{
	// empty
}

co::IComponent* TypeManager_Base::getInterfaceOwner()
{
	return this;
}

void TypeManager_Base::componentRetain()
{
	incrementRefCount();
}

void TypeManager_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* TypeManager_Base::getComponentType()
{
	co::IType* type = co::getType( "co.TypeManager" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* TypeManager_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::ITypeManager>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeManager_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace co
