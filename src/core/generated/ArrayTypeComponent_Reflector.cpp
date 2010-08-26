/*******************************************************************************
** Reflection code generated for type 'co.ArrayTypeComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ArrayType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ArrayTypeComponent_getSize();
co::Component* __ArrayTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ArrayTypeComponent_Reflector : public co::ReflectorBase
{
public:
	ArrayTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~ArrayTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ArrayTypeComponent" );
	}

	co::int32 getSize()
	{
		return __ArrayTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ArrayTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ArrayTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createArrayTypeComponentReflector()
{
    return new ArrayTypeComponent_Reflector;
}

} // namespace co
