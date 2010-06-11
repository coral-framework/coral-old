/*******************************************************************************
** Reflection code generated for type 'co.TypeLoadException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/TypeLoadException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

class TypeLoadException_Reflector : public co::ReflectorBase
{
public:
	TypeLoadException_Reflector()
	{
		// empty
	}

	virtual ~TypeLoadException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::TypeLoadException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeLoadException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createTypeLoadExceptionReflector()
{
    return new TypeLoadException_Reflector;
}

} // namespace __coral
