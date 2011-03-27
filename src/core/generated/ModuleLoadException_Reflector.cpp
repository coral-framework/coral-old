/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ModuleLoadException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::ModuleLoadException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ModuleLoadException);
	}

	void raise( const std::string& message )
	{
		throw co::ModuleLoadException( message );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createModuleLoadExceptionReflector()
{
    return new ModuleLoadException_Reflector;
}

} // namespace co
