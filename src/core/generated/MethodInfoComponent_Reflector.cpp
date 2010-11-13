/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __MethodInfoComponent_getSize();
co::Component* __MethodInfoComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class MethodInfoComponent_Reflector : public co::ReflectorBase
{
public:
	MethodInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~MethodInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.MethodInfoComponent" );
	}

	co::int32 getSize()
	{
		return __MethodInfoComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __MethodInfoComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.MethodInfoComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodInfoComponentReflector()
{
    return new MethodInfoComponent_Reflector;
}

} // namespace co
