/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "Universe_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ lua.Universe provides an interface named 'state', of type lua.IState ------ //

co::InterfaceType* Universe_lua_IState::getInterfaceType()
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

co::Component* Universe_Base::getInterfaceOwner()
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

co::ComponentType* Universe_Base::getComponentType()
{
	co::Type* type = co::getType( "lua.Universe" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* Universe_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void Universe_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace lua
