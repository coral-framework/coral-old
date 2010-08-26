/*******************************************************************************
** Reflection code generated for type 'co.EnumTypeComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/EnumType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __EnumTypeComponent_getSize();
co::Component* __EnumTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class EnumTypeComponent_Reflector : public co::ReflectorBase
{
public:
	EnumTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~EnumTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.EnumTypeComponent" );
	}

	co::int32 getSize()
	{
		return __EnumTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __EnumTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.EnumTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createEnumTypeComponentReflector()
{
    return new EnumTypeComponent_Reflector;
}

} // namespace co
