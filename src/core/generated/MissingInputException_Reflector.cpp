/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/MissingInputException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

class MissingInputException_Reflector : public co::ReflectorBase
{
public:
	MissingInputException_Reflector()
	{
		// empty
	}

	virtual ~MissingInputException_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::MissingInputException>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(co::MissingInputException);
	}

	void raise( const std::string& message )
	{
		throw co::MissingInputException( message );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createMissingInputExceptionReflector()
{
    return new MissingInputException_Reflector;
}

} // namespace co
