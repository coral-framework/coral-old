/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ExceptionType_getSize();
co::IComponent* __ExceptionType_newInstance();

// ------ IReflector ------ //

class ExceptionType_Reflector : public co::ReflectorBase
{
public:
	ExceptionType_Reflector()
	{
		// empty
	}

	virtual ~ExceptionType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.ExceptionType" );
	}

	co::int32 getSize()
	{
		return __ExceptionType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __ExceptionType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ExceptionType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createExceptionTypeIReflector()
{
    return new ExceptionType_Reflector;
}

} // namespace co
