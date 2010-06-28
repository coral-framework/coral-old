/*******************************************************************************
** Reflection code generated for type 'co.MethodInfoComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/MethodInfo.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __MethodInfoComponent_getSize();
co::Component* __MethodInfoComponent_newInstance();

class MethodInfoComponent_Reflector : public co::ReflectorBase
{
public:
	MethodInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~MethodInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.MethodInfoComponent" );
	}

	co::int32 getSize() { return __MethodInfoComponent_getSize(); }
	co::Component* newInstance() { return __MethodInfoComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createMethodInfoComponentReflector()
{
    return new MethodInfoComponent_Reflector;
}

} // namespace __coral
