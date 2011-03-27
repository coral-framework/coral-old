/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Method_getSize();
co::IObject* __Method_newInstance();

// ------ Reflector Component ------ //

class Method_Reflector : public co::ReflectorBase
{
public:
	Method_Reflector()
	{
		// empty
	}

	virtual ~Method_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Method" );
	}

	co::int32 getSize()
	{
		return __Method_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __Method_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Method" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createMethodReflector()
{
    return new Method_Reflector;
}

} // namespace co
