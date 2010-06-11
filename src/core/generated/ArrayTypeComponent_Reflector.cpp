/*******************************************************************************
** Reflection code generated for type 'co.ArrayTypeComponent'
**
** Created: Thu Jun 10 20:53:30 2010
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

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ArrayTypeComponent_getSize();
co::Component* __ArrayTypeComponent_newInstance();

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

	co::int32 getSize() { return __ArrayTypeComponent_getSize(); }
	co::Component* newInstance() { return __ArrayTypeComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createArrayTypeComponentReflector()
{
    return new ArrayTypeComponent_Reflector;
}

} // namespace __coral
