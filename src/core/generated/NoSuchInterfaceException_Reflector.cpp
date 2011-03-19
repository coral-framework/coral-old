/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/NoSuchInterfaceException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::NoSuchInterfaceException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::NoSuchInterfaceException);
	}

	void raise( const std::string& message )
	{
		throw co::NoSuchInterfaceException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createNoSuchInterfaceExceptionIReflector()
{
    return new NoSuchInterfaceException_Reflector;
}

} // namespace co
