/*******************************************************************************
** Reflection code generated for type 'co.StructTypeComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/StructType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __StructTypeComponent_getSize();
co::Component* __StructTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class StructTypeComponent_Reflector : public co::ReflectorBase
{
public:
	StructTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~StructTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.StructTypeComponent" );
	}

	co::int32 getSize()
	{
		return __StructTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __StructTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.StructTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createStructTypeComponentReflector()
{
    return new StructTypeComponent_Reflector;
}

} // namespace co
