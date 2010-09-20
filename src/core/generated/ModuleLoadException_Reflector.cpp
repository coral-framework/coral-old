/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/ModuleLoadException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

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

co::Reflector* __createModuleLoadExceptionReflector()
{
    return new ModuleLoadException_Reflector;
}

} // namespace co
