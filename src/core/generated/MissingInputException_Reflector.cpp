/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/MissingInputException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

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

	co::int32 getSize()
	{
		return sizeof(co::MissingInputException);
	}

	void raise( const std::string& message )
	{
		throw co::MissingInputException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createMissingInputExceptionIReflector()
{
    return new MissingInputException_Reflector;
}

} // namespace co
