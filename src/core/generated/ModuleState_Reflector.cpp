/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ModuleState.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

class ModuleState_Reflector : public co::ReflectorBase
{
public:
	ModuleState_Reflector()
	{
		// empty
	}

	virtual ~ModuleState_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ModuleState>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModuleState);
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createModuleStateReflector()
{
    return new ModuleState_Reflector;
}

} // namespace co
