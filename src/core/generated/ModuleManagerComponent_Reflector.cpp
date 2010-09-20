/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModuleManagerComponent_getSize();
co::Component* __ModuleManagerComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ModuleManagerComponent_Reflector : public co::ReflectorBase
{
public:
	ModuleManagerComponent_Reflector()
	{
		// empty
	}

	virtual ~ModuleManagerComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ModuleManagerComponent" );
	}

	co::int32 getSize()
	{
		return __ModuleManagerComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ModuleManagerComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ModuleManagerComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleManagerComponentReflector()
{
    return new ModuleManagerComponent_Reflector;
}

} // namespace co
