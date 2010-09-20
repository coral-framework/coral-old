/*******************************************************************************
** Reflection code generated for type 'lua.Universe'
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
co::int32 __Universe_getSize();
co::Component* __Universe_newInstance();

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class Universe_Reflector : public co::ReflectorBase
{
public:
	Universe_Reflector()
	{
		moduleRetain();
	}

	virtual ~Universe_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.Universe" );
	}

	co::int32 getSize()
	{
		return __Universe_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __Universe_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.Universe" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createUniverseReflector()
{
    return new Universe_Reflector;
}

} // namespace lua
