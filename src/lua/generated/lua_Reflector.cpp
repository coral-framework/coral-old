/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __lua_getSize();
co::Component* __lua_newInstance();

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class lua_Reflector : public co::ReflectorBase
{
public:
	lua_Reflector()
	{
		moduleRetain();
	}

	virtual ~lua_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.lua" );
	}

	co::int32 getSize()
	{
		return __lua_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __lua_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.lua" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createluaReflector()
{
    return new lua_Reflector;
}

} // namespace lua
