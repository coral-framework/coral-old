/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModuleManager_getSize();
co::IObject* __ModuleManager_newInstance();

// ------ Reflector Component ------ //

class ModuleManager_Reflector : public co::ReflectorBase
{
public:
	ModuleManager_Reflector()
	{
		// empty
	}

	virtual ~ModuleManager_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.ModuleManager" );
	}

	co::int32 getSize()
	{
		return __ModuleManager_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __ModuleManager_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ModuleManager" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createModuleManagerReflector()
{
    return new ModuleManager_Reflector;
}

} // namespace co
