/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <memory>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __Port_getSize();
co::IObject* __Port_newInstance();

// ------ Reflector Component ------ //

class Port_Reflector : public co::ReflectorBase
{
public:
	Port_Reflector()
	{
		// empty
	}

	virtual ~Port_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Port" );
	}

	co::uint32 getSize()
	{
		return __Port_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __Port_newInstance();
		assert( instance->getComponent()->getFullName() == "co.Port" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createPortReflector()
{
    return new Port_Reflector;
}

} // namespace co
