/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Module_getSize();
co::IComponent* __Module_newInstance();

// ------ IReflector ------ //

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

	co::int32 getSize()
	{
		return __Module_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __Module_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Module" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createModuleIReflector()
{
    return new Module_Reflector;
}

} // namespace co
