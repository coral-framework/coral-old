/*******************************************************************************
** Reflection code generated for type 'co.LifeCycleException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/LifeCycleException.h>
#include <co/reserved/ReflectorBase.h>

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

namespace __coral {

co::Reflector* createLifeCycleExceptionReflector()
{
    return new LifeCycleException_Reflector;
}

} // namespace __coral
