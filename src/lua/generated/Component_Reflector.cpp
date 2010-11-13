/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Component_getSize();
co::Component* __Component_newInstance();

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class Component_Reflector : public co::ReflectorBase
{
public:
	Component_Reflector()
	{
		moduleRetain();
	}

	virtual ~Component_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.Component" );
	}

	co::int32 getSize()
	{
		return __Component_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __Component_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.Component" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createComponentReflector()
{
    return new Component_Reflector;
}

} // namespace lua
