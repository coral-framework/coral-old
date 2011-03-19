/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModuleManager_getSize();
co::IComponent* __ModuleManager_newInstance();

// ------ IReflector ------ //

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

	co::IComponent* newInstance()
	{
		co::IComponent* component = __ModuleManager_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ModuleManager" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createModuleManagerIReflector()
{
    return new ModuleManager_Reflector;
}

} // namespace co
