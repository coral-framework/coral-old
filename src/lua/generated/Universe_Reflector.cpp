/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Universe_getSize();
co::IComponent* __Universe_newInstance();

void moduleRetain();
void moduleRelease();

// ------ IReflector ------ //

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

	co::IType* getType()
	{
		return co::getType( "lua.Universe" );
	}

	co::int32 getSize()
	{
		return __Universe_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __Universe_newInstance();
		assert( component->getComponentType()->getFullName() == "lua.Universe" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createUniverseIReflector()
{
    return new Universe_Reflector;
}

} // namespace lua
