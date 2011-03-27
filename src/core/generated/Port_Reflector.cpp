/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Port_getSize();
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

	co::int32 getSize()
	{
		return __Port_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __Port_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Port" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createPortReflector()
{
    return new Port_Reflector;
}

} // namespace co
