/*******************************************************************************
** Reflection code generated for type 'co.IllegalNameException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/IllegalNameException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

class IllegalNameException_Reflector : public co::ReflectorBase
{
public:
	IllegalNameException_Reflector()
	{
		// empty
	}

	virtual ~IllegalNameException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::IllegalNameException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalNameException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createIllegalNameExceptionReflector()
{
    return new IllegalNameException_Reflector;
}

} // namespace __coral
