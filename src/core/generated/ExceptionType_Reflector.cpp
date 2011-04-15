/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __ExceptionType_getSize();
co::IObject* __ExceptionType_newInstance();

// ------ Reflector Component ------ //

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

	co::uint32 getSize()
	{
		return __ExceptionType_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __ExceptionType_newInstance();
		assert( instance->getComponent()->getFullName() == "co.ExceptionType" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createExceptionTypeReflector()
{
    return new ExceptionType_Reflector;
}

} // namespace co
