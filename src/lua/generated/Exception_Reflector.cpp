/*******************************************************************************
** Reflection code generated for type 'lua.Exception'
**
** Created: Mon Sep 20 12:13:52 2010
**      by: Coral Compiler version 0.3.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <lua/Exception.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

#if !defined( CORAL_COMPILER_OUTPUT_REVISION )
#error "The header file <co/Config.h> was not included."
#elif CORAL_COMPILER_OUTPUT_REVISION != 1
#error "This file was generated using the Coral Compiler v0.3.0. It"
#error "cannot be used with the include files from this version of Coral."
#error "(The code generation rules have changed too much.)"
#endif

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class Exception_Reflector : public co::ReflectorBase
{
public:
	Exception_Reflector()
	{
		moduleRetain();
	}

	virtual ~Exception_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::typeOf<lua::Exception>::get();
	}

	co::int32 getSize()
	{
		return sizeof(lua::Exception);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createExceptionReflector()
{
    return new Exception_Reflector;
}

} // namespace lua
