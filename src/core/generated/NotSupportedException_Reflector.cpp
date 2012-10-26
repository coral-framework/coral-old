/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/NotSupportedException.h>
#include <co/reserved/ReflectorBase.h>
#include <memory>

namespace co {

// ------ Reflector Component ------ //

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

	co::uint32 getSize()
	{
		return sizeof(co::NotSupportedException);
	}

	void raise( const std::string& message )
	{
		throw co::NotSupportedException( message );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createNotSupportedExceptionReflector()
{
    return new NotSupportedException_Reflector;
}

} // namespace co
