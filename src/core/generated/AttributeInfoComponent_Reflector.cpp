/*******************************************************************************
** Reflection code generated for type 'co.AttributeInfoComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/AttributeInfo.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __AttributeInfoComponent_getSize();
co::Component* __AttributeInfoComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class AttributeInfoComponent_Reflector : public co::ReflectorBase
{
public:
	AttributeInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~AttributeInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.AttributeInfoComponent" );
	}

	co::int32 getSize()
	{
		return __AttributeInfoComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __AttributeInfoComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.AttributeInfoComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createAttributeInfoComponentReflector()
{
    return new AttributeInfoComponent_Reflector;
}

} // namespace co
