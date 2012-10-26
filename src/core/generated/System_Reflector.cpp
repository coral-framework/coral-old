/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <memory>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __System_getSize();
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

	co::uint32 getSize()
	{
		return __System_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __System_newInstance();
		assert( instance->getComponent()->getFullName() == "co.System" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createSystemReflector()
{
    return new System_Reflector;
}

} // namespace co
