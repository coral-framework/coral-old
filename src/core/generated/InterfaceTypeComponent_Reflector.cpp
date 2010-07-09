/*******************************************************************************
** Reflection code generated for type 'co.InterfaceTypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/InterfaceType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceTypeComponent_getSize();
co::Component* __InterfaceTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class InterfaceTypeComponent_Reflector : public co::ReflectorBase
{
public:
	InterfaceTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~InterfaceTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.InterfaceTypeComponent" );
	}

	co::int32 getSize()
	{
		return __InterfaceTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __InterfaceTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.InterfaceTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createInterfaceTypeComponentReflector()
{
    return new InterfaceTypeComponent_Reflector;
}

} // namespace co
