/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __MethodInfo_getSize();
co::IComponent* __MethodInfo_newInstance();

// ------ IReflector ------ //

class MethodInfo_Reflector : public co::ReflectorBase
{
public:
	MethodInfo_Reflector()
	{
		// empty
	}

	virtual ~MethodInfo_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.MethodInfo" );
	}

	co::int32 getSize()
	{
		return __MethodInfo_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __MethodInfo_newInstance();
		assert( component->getComponentType()->getFullName() == "co.MethodInfo" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createMethodInfoIReflector()
{
    return new MethodInfo_Reflector;
}

} // namespace co
