/*******************************************************************************
** Reflection code generated for type 'co.TypeComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/Type.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeComponent_getSize();
co::Component* __TypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class TypeComponent_Reflector : public co::ReflectorBase
{
public:
	TypeComponent_Reflector()
	{
		// empty
	}

	virtual ~TypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.TypeComponent" );
	}

	co::int32 getSize()
	{
		return __TypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __TypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeComponentReflector()
{
    return new TypeComponent_Reflector;
}

} // namespace co
