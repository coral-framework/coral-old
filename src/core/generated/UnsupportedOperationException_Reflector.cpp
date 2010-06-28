/*******************************************************************************
** Reflection code generated for type 'co.UnsupportedOperationException'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/UnsupportedOperationException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

class UnsupportedOperationException_Reflector : public co::ReflectorBase
{
public:
	UnsupportedOperationException_Reflector()
	{
		// empty
	}

	virtual ~UnsupportedOperationException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::UnsupportedOperationException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::UnsupportedOperationException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createUnsupportedOperationExceptionReflector()
{
    return new UnsupportedOperationException_Reflector;
}

} // namespace __coral
