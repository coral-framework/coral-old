/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ExceptionTypeComponent_getSize();
co::Component* __ExceptionTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ExceptionTypeComponent_Reflector : public co::ReflectorBase
{
public:
	ExceptionTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~ExceptionTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ExceptionTypeComponent" );
	}

	co::int32 getSize()
	{
		return __ExceptionTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ExceptionTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ExceptionTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createExceptionTypeComponentReflector()
{
    return new ExceptionTypeComponent_Reflector;
}

} // namespace co
