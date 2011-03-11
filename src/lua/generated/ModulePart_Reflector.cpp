/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModulePart_getSize();
co::Component* __ModulePart_newInstance();

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class ModulePart_Reflector : public co::ReflectorBase
{
public:
	ModulePart_Reflector()
	{
		moduleRetain();
	}

	virtual ~ModulePart_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.ModulePart" );
	}

	co::int32 getSize()
	{
		return __ModulePart_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ModulePart_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.ModulePart" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModulePartReflector()
{
    return new ModulePart_Reflector;
}

} // namespace lua
