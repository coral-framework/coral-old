/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/NotSupportedException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

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

	co::Type* getType()
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

// ------ Reflector Creation Function ------ //

co::Reflector* __createNotSupportedExceptionReflector()
{
    return new NotSupportedException_Reflector;
}

} // namespace co
