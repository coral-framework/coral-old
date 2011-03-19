/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/NotSupportedException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

class NotSupportedException_Reflector : public co::ReflectorBase
{
public:
	NotSupportedException_Reflector()
	{
		// empty
	}

	virtual ~NotSupportedException_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::NotSupportedException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::NotSupportedException);
	}

	void raise( const std::string& message )
	{
		throw co::NotSupportedException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createNotSupportedExceptionIReflector()
{
    return new NotSupportedException_Reflector;
}

} // namespace co
