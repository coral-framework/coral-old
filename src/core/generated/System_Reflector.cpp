/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __System_getSize();
co::IObject* __System_newInstance();

// ------ Reflector Component ------ //

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

	co::IObject* newInstance()
	{
		co::IObject* component = __System_newInstance();
		assert( component->getComponentType()->getFullName() == "co.System" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createSystemReflector()
{
    return new System_Reflector;
}

} // namespace co
