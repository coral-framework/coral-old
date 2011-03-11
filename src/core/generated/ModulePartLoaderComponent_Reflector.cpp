/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModulePartLoaderComponent_getSize();
co::Component* __ModulePartLoaderComponent_newInstance();

// ------ Reflector ------ //

class ModulePartLoaderComponent_Reflector : public co::ReflectorBase
{
public:
	ModulePartLoaderComponent_Reflector()
	{
		// empty
	}

	virtual ~ModulePartLoaderComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ModulePartLoaderComponent" );
	}

	co::int32 getSize()
	{
		return __ModulePartLoaderComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ModulePartLoaderComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ModulePartLoaderComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModulePartLoaderComponentReflector()
{
    return new ModulePartLoaderComponent_Reflector;
}

} // namespace co
