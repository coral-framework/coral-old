/*******************************************************************************
** Reflection code generated for type 'co.ModuleState'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ModuleState.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

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

	co::Type* getType()
	{
		return co::typeOf<co::ModuleState>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModuleState);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createModuleStateReflector()
{
    return new ModuleState_Reflector;
}

} // namespace __coral
