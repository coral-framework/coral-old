/*******************************************************************************
** Reflection code generated for type 'co.ModuleState'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ModuleState.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

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

co::Reflector* __createModuleStateReflector()
{
    return new ModuleState_Reflector;
}

} // namespace co
