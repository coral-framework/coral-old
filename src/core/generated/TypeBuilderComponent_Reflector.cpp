/*******************************************************************************
** Reflection code generated for type 'co.TypeBuilderComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/TypeBuilder.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeBuilderComponent_getSize();
co::Component* __TypeBuilderComponent_newInstance();

class TypeBuilderComponent_Reflector : public co::ReflectorBase
{
public:
	TypeBuilderComponent_Reflector()
	{
		// empty
	}

	virtual ~TypeBuilderComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.TypeBuilderComponent" );
	}

	co::int32 getSize() { return __TypeBuilderComponent_getSize(); }
	co::Component* newInstance() { return __TypeBuilderComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createTypeBuilderComponentReflector()
{
    return new TypeBuilderComponent_Reflector;
}

} // namespace __coral
