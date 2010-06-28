/*******************************************************************************
** Reflection code generated for type 'co.ComponentTypeComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ComponentType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ComponentTypeComponent_getSize();
co::Component* __ComponentTypeComponent_newInstance();

class ComponentTypeComponent_Reflector : public co::ReflectorBase
{
public:
	ComponentTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~ComponentTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ComponentTypeComponent" );
	}

	co::int32 getSize() { return __ComponentTypeComponent_getSize(); }
	co::Component* newInstance() { return __ComponentTypeComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createComponentTypeComponentReflector()
{
    return new ComponentTypeComponent_Reflector;
}

} // namespace __coral
