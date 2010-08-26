/*******************************************************************************
** Reflection code generated for type 'co.ParameterInfoComponent'
**
** Created: Wed Aug 25 16:31:31 2010
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
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ParameterInfoComponent_getSize();
co::Component* __ParameterInfoComponent_newInstance();

namespace co {

// ------ Reflector ------ //

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

	co::int32 getSize()
	{
		return __ParameterInfoComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ParameterInfoComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ParameterInfoComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createParameterInfoComponentReflector()
{
    return new ParameterInfoComponent_Reflector;
}

} // namespace co
