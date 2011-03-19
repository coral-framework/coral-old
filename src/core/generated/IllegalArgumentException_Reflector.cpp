/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::IllegalArgumentException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalArgumentException);
	}

	void raise( const std::string& message )
	{
		throw co::IllegalArgumentException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIllegalArgumentExceptionIReflector()
{
    return new IllegalArgumentException_Reflector;
}

} // namespace co
