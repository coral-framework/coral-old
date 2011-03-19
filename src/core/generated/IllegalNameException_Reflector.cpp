/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IllegalNameException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::IllegalNameException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalNameException);
	}

	void raise( const std::string& message )
	{
		throw co::IllegalNameException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIllegalNameExceptionIReflector()
{
    return new IllegalNameException_Reflector;
}

} // namespace co
