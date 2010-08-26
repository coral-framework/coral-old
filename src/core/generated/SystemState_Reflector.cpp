/*******************************************************************************
** Reflection code generated for type 'co.SystemState'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

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
