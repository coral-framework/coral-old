/*******************************************************************************
** Reflection code generated for type 'lua.ModulePartLoader'
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
co::int32 __ModulePartLoader_getSize();
co::Component* __ModulePartLoader_newInstance();

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class ModulePartLoader_Reflector : public co::ReflectorBase
{
public:
	ModulePartLoader_Reflector()
	{
		moduleRetain();
	}

	virtual ~ModulePartLoader_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.ModulePartLoader" );
	}

	co::int32 getSize()
	{
		return __ModulePartLoader_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ModulePartLoader_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.ModulePartLoader" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModulePartLoaderReflector()
{
    return new ModulePartLoader_Reflector;
}

} // namespace lua
