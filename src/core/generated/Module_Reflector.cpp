/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <memory>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __Module_getSize();
co::IObject* __Module_newInstance();

// ------ Reflector Component ------ //

class Module_Reflector : public co::ReflectorBase
{
public:
	Module_Reflector()
	{
		// empty
	}

	virtual ~Module_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Module" );
	}

	co::uint32 getSize()
	{
		return __Module_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __Module_newInstance();
		assert( instance->getComponent()->getFullName() == "co.Module" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createModuleReflector()
{
    return new Module_Reflector;
}

} // namespace co
