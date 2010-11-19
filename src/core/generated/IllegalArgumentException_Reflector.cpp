/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class IllegalArgumentException_Reflector : public co::ReflectorBase
{
public:
	IllegalArgumentException_Reflector()
	{
		// empty
	}

	virtual ~IllegalArgumentException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::IllegalArgumentException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalArgumentException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createIllegalArgumentExceptionReflector()
{
    return new IllegalArgumentException_Reflector;
}

} // namespace co
