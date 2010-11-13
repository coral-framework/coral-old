/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModuleComponent_getSize();
co::Component* __ModuleComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ModuleComponent_Reflector : public co::ReflectorBase
{
public:
	ModuleComponent_Reflector()
	{
		// empty
	}

	virtual ~ModuleComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ModuleComponent" );
	}

	co::int32 getSize()
	{
		return __ModuleComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ModuleComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ModuleComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModuleComponentReflector()
{
    return new ModuleComponent_Reflector;
}

} // namespace co
