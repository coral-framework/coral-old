/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Universe_Base.h"
#include <co/Coral.h>
#include <co/IComponentType.h>
#include <co/IInterfaceInfo.h>
#include <co/IInterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Universe provides an interface named 'state', of type lua.IState ------ //

co::IInterfaceType* Universe_lua_IState::getInterfaceType()
{
	return co::typeOf<lua::IState>::get();
}

const std::string& Universe_lua_IState::getInterfaceName()
{
	static const std::string s_interfaceName( "state" );
	return s_interfaceName;
}

// ------ Universe_Base ------ //

Universe_Base::Universe_Base()
{
	moduleRetain();
}

Universe_Base::~Universe_Base()
{
	moduleRelease();
}

co::IComponent* Universe_Base::getInterfaceOwner()
{
	return this;
}

void Universe_Base::componentRetain()
{
	incrementRefCount();
}

void Universe_Base::componentRelease()
{
	decrementRefCount();
}

co::IComponentType* Universe_Base::getComponentType()
{
	co::IType* type = co::getType( "lua.Universe" );
	assert( dynamic_cast<co::IComponentType*>( type ) );
	return static_cast<co::IComponentType*>( type );
}

co::Interface* Universe_Base::getInterface( co::IInterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, lua::IState>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void Universe_Base::setReceptacle( co::IInterfaceInfo* receptacle, co::Interface* facet )
{
	checkValidReceptacle( receptacle );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( facet );
}

} // namespace lua
