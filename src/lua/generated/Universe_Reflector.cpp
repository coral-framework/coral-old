/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Universe_getSize();
co::Component* __Universe_newInstance();

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Reflector ------ //

class Universe_Reflector : public co::ReflectorBase
{
public:
	Universe_Reflector()
	{
		moduleRetain();
	}

	virtual ~Universe_Reflector()
	{
		moduleRelease();
	}

	co::Type* getType()
	{
		return co::getType( "lua.Universe" );
	}

	co::int32 getSize()
	{
		return __Universe_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __Universe_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.Universe" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createUniverseReflector()
{
    return new Universe_Reflector;
}

} // namespace lua
