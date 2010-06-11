/*******************************************************************************
** Reflection code generated for type 'co.NativeClassTypeComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/NativeClassType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __NativeClassTypeComponent_getSize();
co::Component* __NativeClassTypeComponent_newInstance();

class NativeClassTypeComponent_Reflector : public co::ReflectorBase
{
public:
	NativeClassTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~NativeClassTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.NativeClassTypeComponent" );
	}

	co::int32 getSize() { return __NativeClassTypeComponent_getSize(); }
	co::Component* newInstance() { return __NativeClassTypeComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createNativeClassTypeComponentReflector()
{
    return new NativeClassTypeComponent_Reflector;
}

} // namespace __coral
