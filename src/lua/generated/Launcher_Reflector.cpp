/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Launcher_getSize();
co::Component* __Launcher_newInstance();

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class Launcher_Reflector : public co::ReflectorBase
{
public:
	Launcher_Reflector()
	{
		moduleRetain();
	}

	virtual ~Launcher_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.Launcher" );
	}

	co::int32 getSize()
	{
		return __Launcher_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __Launcher_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.Launcher" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createLauncherReflector()
{
    return new Launcher_Reflector;
}

} // namespace lua
