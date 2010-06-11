/*******************************************************************************
** Reflection code generated for type 'co.MethodBuilderComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/MethodBuilder.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __MethodBuilderComponent_getSize();
co::Component* __MethodBuilderComponent_newInstance();

class MethodBuilderComponent_Reflector : public co::ReflectorBase
{
public:
	MethodBuilderComponent_Reflector()
	{
		// empty
	}

	virtual ~MethodBuilderComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.MethodBuilderComponent" );
	}

	co::int32 getSize() { return __MethodBuilderComponent_getSize(); }
	co::Component* newInstance() { return __MethodBuilderComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createMethodBuilderComponentReflector()
{
    return new MethodBuilderComponent_Reflector;
}

} // namespace __coral
