/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/MissingServiceException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class MissingServiceException_Reflector : public co::ReflectorBase
{
public:
	MissingServiceException_Reflector()
	{
		// empty
	}

	virtual ~MissingServiceException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MissingServiceException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MissingServiceException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMissingServiceExceptionReflector()
{
    return new MissingServiceException_Reflector;
}

} // namespace co
