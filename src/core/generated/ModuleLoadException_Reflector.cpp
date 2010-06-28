/*******************************************************************************
** Reflection code generated for type 'co.ModuleLoadException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ModuleLoadException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

class ModuleLoadException_Reflector : public co::ReflectorBase
{
public:
	ModuleLoadException_Reflector()
	{
		// empty
	}

	virtual ~ModuleLoadException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ModuleLoadException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModuleLoadException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createModuleLoadExceptionReflector()
{
    return new ModuleLoadException_Reflector;
}

} // namespace __coral
