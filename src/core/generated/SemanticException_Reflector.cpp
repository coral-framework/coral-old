/*******************************************************************************
** Reflection code generated for type 'co.SemanticException'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/SemanticException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class SemanticException_Reflector : public co::ReflectorBase
{
public:
	SemanticException_Reflector()
	{
		// empty
	}

	virtual ~SemanticException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::SemanticException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::SemanticException);
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createSemanticExceptionReflector()
{
    return new SemanticException_Reflector;
}

} // namespace co
