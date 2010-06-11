/*******************************************************************************
** Reflection code generated for type 'co.SemanticException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/SemanticException.h>
#include <co/reserved/ReflectorBase.h>

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

namespace __coral {

co::Reflector* createSemanticExceptionReflector()
{
    return new SemanticException_Reflector;
}

} // namespace __coral
