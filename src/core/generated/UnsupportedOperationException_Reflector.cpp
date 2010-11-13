/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/UnsupportedOperationException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class UnsupportedOperationException_Reflector : public co::ReflectorBase
{
public:
	UnsupportedOperationException_Reflector()
	{
		// empty
	}

	virtual ~UnsupportedOperationException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::UnsupportedOperationException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::UnsupportedOperationException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createUnsupportedOperationExceptionReflector()
{
    return new UnsupportedOperationException_Reflector;
}

} // namespace co
