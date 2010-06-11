/*******************************************************************************
** Reflection code generated for type 'co.NoSuchInterfaceException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/NoSuchInterfaceException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

class NoSuchInterfaceException_Reflector : public co::ReflectorBase
{
public:
	NoSuchInterfaceException_Reflector()
	{
		// empty
	}

	virtual ~NoSuchInterfaceException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::NoSuchInterfaceException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::NoSuchInterfaceException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createNoSuchInterfaceExceptionReflector()
{
    return new NoSuchInterfaceException_Reflector;
}

} // namespace __coral
