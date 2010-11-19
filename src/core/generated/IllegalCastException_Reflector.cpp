/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IllegalCastException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class IllegalCastException_Reflector : public co::ReflectorBase
{
public:
	IllegalCastException_Reflector()
	{
		// empty
	}

	virtual ~IllegalCastException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::IllegalCastException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalCastException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createIllegalCastExceptionReflector()
{
    return new IllegalCastException_Reflector;
}

} // namespace co
