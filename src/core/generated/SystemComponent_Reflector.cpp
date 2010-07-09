/*******************************************************************************
** Reflection code generated for type 'co.SystemComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/System.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __SystemComponent_getSize();
co::Component* __SystemComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class SystemComponent_Reflector : public co::ReflectorBase
{
public:
	SystemComponent_Reflector()
	{
		// empty
	}

	virtual ~SystemComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.SystemComponent" );
	}

	co::int32 getSize()
	{
		return __SystemComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __SystemComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.SystemComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createSystemComponentReflector()
{
    return new SystemComponent_Reflector;
}

} // namespace co
