/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/MissingServiceException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::MissingServiceException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MissingServiceException);
	}

	void raise( const std::string& message )
	{
		throw co::MissingServiceException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createMissingServiceExceptionIReflector()
{
    return new MissingServiceException_Reflector;
}

} // namespace co
