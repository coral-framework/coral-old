/*******************************************************************************
** Reflection code generated for type 'co.LifeCycleException'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

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
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createLifeCycleExceptionReflector()
{
    return new LifeCycleException_Reflector;
}

} // namespace co
