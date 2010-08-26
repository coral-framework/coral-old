/*******************************************************************************
** Reflection code generated for type 'co.IllegalArgumentException'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class IllegalArgumentException_Reflector : public co::ReflectorBase
{
public:
	IllegalArgumentException_Reflector()
	{
		// empty
	}

	virtual ~IllegalArgumentException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::IllegalArgumentException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IllegalArgumentException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createIllegalArgumentExceptionReflector()
{
    return new IllegalArgumentException_Reflector;
}

} // namespace co
