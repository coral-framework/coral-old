/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/LifeCycleException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

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

	co::Type* getType()
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

// ------ Reflector Creation Function ------ //

co::Reflector* __createLifeCycleExceptionReflector()
{
    return new LifeCycleException_Reflector;
}

} // namespace co
