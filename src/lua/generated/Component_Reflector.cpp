/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __Component_getSize();
co::IObject* __Component_newInstance();

void moduleRetain();
void moduleRelease();

// ------ Reflector Component ------ //

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

	co::IType* getType()
	{
		return co::getType( "lua.Component" );
	}

	co::uint32 getSize()
	{
		return __Component_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __Component_newInstance();
		assert( instance->getComponent()->getFullName() == "lua.Component" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createComponentReflector()
{
    return new Component_Reflector;
}

} // namespace lua
