/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Component_getSize();
co::IObject* __Component_newInstance();

// ------ Reflector Component ------ //

class Component_Reflector : public co::ReflectorBase
{
public:
	Component_Reflector()
	{
		// empty
	}

	virtual ~Component_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Component" );
	}

	co::int32 getSize()
	{
		return __Component_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __Component_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Component" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createComponentReflector()
{
    return new Component_Reflector;
}

} // namespace co
