/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IllegalCastException.h>
#include <co/reserved/ReflectorBase.h>
#include <memory>

namespace co {

// ------ Reflector Component ------ //

class IllegalCastException_Reflector : public co::ReflectorBase
{
public:
	IllegalCastException_Reflector()
	{
		// empty
	}

	virtual ~IllegalCastException_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IllegalCastException>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(co::IllegalCastException);
	}

	void raise( const std::string& message )
	{
		throw co::IllegalCastException( message );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIllegalCastExceptionReflector()
{
    return new IllegalCastException_Reflector;
}

} // namespace co
