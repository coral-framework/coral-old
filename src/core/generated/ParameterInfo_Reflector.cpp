/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ParameterInfo_getSize();
co::IComponent* __ParameterInfo_newInstance();

// ------ IReflector ------ //

class ParameterInfo_Reflector : public co::ReflectorBase
{
public:
	ParameterInfo_Reflector()
	{
		// empty
	}

	virtual ~ParameterInfo_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.ParameterInfo" );
	}

	co::int32 getSize()
	{
		return __ParameterInfo_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __ParameterInfo_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ParameterInfo" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createParameterInfoIReflector()
{
    return new ParameterInfo_Reflector;
}

} // namespace co
