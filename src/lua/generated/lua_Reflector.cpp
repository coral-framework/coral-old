/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __lua_getSize();
co::IComponent* __lua_newInstance();

void moduleRetain();
void moduleRelease();

// ------ IReflector ------ //

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

	co::IType* getType()
	{
		return co::getType( "lua.lua" );
	}

	co::int32 getSize()
	{
		return __lua_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __lua_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.lua" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createluaIReflector()
{
    return new lua_Reflector;
}

} // namespace lua
