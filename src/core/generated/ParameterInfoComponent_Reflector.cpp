/*******************************************************************************
** Reflection code generated for type 'co.ParameterInfoComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ParameterInfo.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ParameterInfoComponent_getSize();
co::Component* __ParameterInfoComponent_newInstance();

class ParameterInfoComponent_Reflector : public co::ReflectorBase
{
public:
	ParameterInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~ParameterInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ParameterInfoComponent" );
	}

	co::int32 getSize() { return __ParameterInfoComponent_getSize(); }
	co::Component* newInstance() { return __ParameterInfoComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createParameterInfoComponentReflector()
{
    return new ParameterInfoComponent_Reflector;
}

} // namespace __coral
