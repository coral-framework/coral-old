/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/NoSuchInterfaceException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class NoSuchInterfaceException_Reflector : public co::ReflectorBase
{
public:
	NoSuchInterfaceException_Reflector()
	{
		// empty
	}

	virtual ~NoSuchInterfaceException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::NoSuchInterfaceException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::NoSuchInterfaceException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createNoSuchInterfaceExceptionReflector()
{
    return new NoSuchInterfaceException_Reflector;
}

} // namespace co
