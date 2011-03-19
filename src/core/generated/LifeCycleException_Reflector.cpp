/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/LifeCycleException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ IReflector ------ //

class LifeCycleException_Reflector : public co::ReflectorBase
{
public:
	LifeCycleException_Reflector()
	{
		// empty
	}

	virtual ~LifeCycleException_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::LifeCycleException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::LifeCycleException);
	}

	void raise( const std::string& message )
	{
		throw co::LifeCycleException( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createLifeCycleExceptionIReflector()
{
    return new LifeCycleException_Reflector;
}

} // namespace co
