/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModulePartLoader_getSize();
co::IObject* __ModulePartLoader_newInstance();

void moduleRetain();
void moduleRelease();

// ------ Reflector Component ------ //

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

	co::IType* getType()
	{
		return co::getType( "lua.ModulePartLoader" );
	}

	co::int32 getSize()
	{
		return __ModulePartLoader_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __ModulePartLoader_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.ModulePartLoader" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createModulePartLoaderReflector()
{
    return new ModulePartLoader_Reflector;
}

} // namespace lua
