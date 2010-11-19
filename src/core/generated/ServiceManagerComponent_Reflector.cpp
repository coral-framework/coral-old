/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ServiceManagerComponent_getSize();
co::Component* __ServiceManagerComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ServiceManagerComponent_Reflector : public co::ReflectorBase
{
public:
	ServiceManagerComponent_Reflector()
	{
		// empty
	}

	virtual ~ServiceManagerComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ServiceManagerComponent" );
	}

	co::int32 getSize()
	{
		return __ServiceManagerComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ServiceManagerComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ServiceManagerComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createServiceManagerComponentReflector()
{
    return new ServiceManagerComponent_Reflector;
}

} // namespace co
