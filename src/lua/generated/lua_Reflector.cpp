/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __lua_getSize();
co::IObject* __lua_newInstance();

void moduleRetain();
void moduleRelease();

// ------ Reflector Component ------ //

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

	co::uint32 getSize()
	{
		return __lua_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __lua_newInstance();
		assert( instance->getComponent()->getFullName() == "lua.lua" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createluaReflector()
{
    return new lua_Reflector;
}

} // namespace lua
