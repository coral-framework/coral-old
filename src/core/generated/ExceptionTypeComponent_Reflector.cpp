/*******************************************************************************
** Reflection code generated for type 'co.ExceptionTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ExceptionType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ExceptionTypeComponent_getSize();
co::Component* __ExceptionTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ExceptionTypeComponent_Reflector : public co::ReflectorBase
{
public:
	ExceptionTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~ExceptionTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ExceptionTypeComponent" );
	}

	co::int32 getSize()
	{
		return __ExceptionTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ExceptionTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ExceptionTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createExceptionTypeComponentReflector()
{
    return new ExceptionTypeComponent_Reflector;
}

} // namespace co
