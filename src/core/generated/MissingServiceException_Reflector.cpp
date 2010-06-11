/*******************************************************************************
** Reflection code generated for type 'co.MissingServiceException'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/MissingServiceException.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

class MissingServiceException_Reflector : public co::ReflectorBase
{
public:
	MissingServiceException_Reflector()
	{
		// empty
	}

	virtual ~MissingServiceException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MissingServiceException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MissingServiceException);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createMissingServiceExceptionReflector()
{
    return new MissingServiceException_Reflector;
}

} // namespace __coral
