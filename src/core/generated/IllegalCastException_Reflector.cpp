/*******************************************************************************
** Reflection code generated for type 'co.IllegalCastException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/IllegalCastException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

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

	co::Type* getType()
	{
		return co::typeOf<co::IllegalCastException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalCastException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createIllegalCastExceptionReflector()
{
    return new IllegalCastException_Reflector;
}

} // namespace __coral
