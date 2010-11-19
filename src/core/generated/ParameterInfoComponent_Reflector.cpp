/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ParameterInfoComponent_getSize();
co::Component* __ParameterInfoComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ParameterInfoComponent_Reflector : public co::ReflectorBase
{
public:
	ParameterInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~ParameterInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ParameterInfoComponent" );
	}

	co::int32 getSize()
	{
		return __ParameterInfoComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ParameterInfoComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ParameterInfoComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createParameterInfoComponentReflector()
{
    return new ParameterInfoComponent_Reflector;
}

} // namespace co
