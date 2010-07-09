/*******************************************************************************
** Reflection code generated for type 'co.MissingInputException'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/MissingInputException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class MissingInputException_Reflector : public co::ReflectorBase
{
public:
	MissingInputException_Reflector()
	{
		// empty
	}

	virtual ~MissingInputException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MissingInputException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MissingInputException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMissingInputExceptionReflector()
{
    return new MissingInputException_Reflector;
}

} // namespace co
