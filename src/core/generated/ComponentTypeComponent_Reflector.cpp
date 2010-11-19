/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ComponentTypeComponent_getSize();
co::Component* __ComponentTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ComponentTypeComponent_Reflector : public co::ReflectorBase
{
public:
	ComponentTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~ComponentTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ComponentTypeComponent" );
	}

	co::int32 getSize()
	{
		return __ComponentTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ComponentTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ComponentTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createComponentTypeComponentReflector()
{
    return new ComponentTypeComponent_Reflector;
}

} // namespace co
