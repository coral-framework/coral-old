/*******************************************************************************
** Reflection code generated for type 'co.ComponentTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
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
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ComponentTypeComponent_getSize();
co::Component* __ComponentTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

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

	co::int32 getSize()
	{
		return __ComponentTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ComponentTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ComponentTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createComponentTypeComponentReflector()
{
    return new ComponentTypeComponent_Reflector;
}

} // namespace co
