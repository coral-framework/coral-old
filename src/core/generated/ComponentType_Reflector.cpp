/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ComponentType_getSize();
co::IComponent* __ComponentType_newInstance();

// ------ IReflector ------ //

class ComponentType_Reflector : public co::ReflectorBase
{
public:
	ComponentType_Reflector()
	{
		// empty
	}

	virtual ~ComponentType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.ComponentType" );
	}

	co::int32 getSize()
	{
		return __ComponentType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __ComponentType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ComponentType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createComponentTypeIReflector()
{
    return new ComponentType_Reflector;
}

} // namespace co
