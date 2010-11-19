/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IllegalNameException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class IllegalNameException_Reflector : public co::ReflectorBase
{
public:
	IllegalNameException_Reflector()
	{
		// empty
	}

	virtual ~IllegalNameException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::IllegalNameException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalNameException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createIllegalNameExceptionReflector()
{
    return new IllegalNameException_Reflector;
}

} // namespace co
