/*******************************************************************************
** Reflection code generated for type 'co.SystemState'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/SystemState.h>
#include <co/reserved/ReflectorBase.h>

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

namespace __coral {

co::Reflector* createSystemStateReflector()
{
    return new SystemState_Reflector;
}

} // namespace __coral
