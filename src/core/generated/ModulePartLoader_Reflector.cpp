/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModulePartLoader_getSize();
co::IObject* __ModulePartLoader_newInstance();

// ------ Reflector Component ------ //

class ModulePartLoader_Reflector : public co::ReflectorBase
{
public:
	ModulePartLoader_Reflector()
	{
		// empty
	}

	virtual ~ModulePartLoader_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.ModulePartLoader" );
	}

	co::int32 getSize()
	{
		return __ModulePartLoader_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __ModulePartLoader_newInstance();
		assert( instance->getComponent()->getFullName() == "co.ModulePartLoader" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createModulePartLoaderReflector()
{
    return new ModulePartLoader_Reflector;
}

} // namespace co
