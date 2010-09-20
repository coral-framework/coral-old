/*******************************************************************************
** Reflection code generated for type 'lua.lua'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <cassert>

#if !defined( CORAL_COMPILER_OUTPUT_REVISION )
#error "The header file <co/Config.h> was not included."
#elif CORAL_COMPILER_OUTPUT_REVISION != 1
#error "This file was generated using the Coral Compiler v0.3.0. It"
#error "cannot be used with the include files from this version of Coral."
#error "(The code generation rules have changed too much.)"
#endif

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
