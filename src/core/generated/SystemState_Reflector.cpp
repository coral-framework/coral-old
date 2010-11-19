/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/SystemState.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class SystemState_Reflector : public co::ReflectorBase
{
public:
	SystemState_Reflector()
	{
		// empty
	}

	virtual ~SystemState_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::SystemState>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::SystemState);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createSystemStateReflector()
{
    return new SystemState_Reflector;
}

} // namespace co
