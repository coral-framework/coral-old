/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __System_getSize();
co::IComponent* __System_newInstance();

// ------ IReflector ------ //

class System_Reflector : public co::ReflectorBase
{
public:
	System_Reflector()
	{
		// empty
	}

	virtual ~System_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.System" );
	}

	co::int32 getSize()
	{
		return __System_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __System_newInstance();
		assert( component->getComponentType()->getFullName() == "co.System" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createSystemIReflector()
{
    return new System_Reflector;
}

} // namespace co
