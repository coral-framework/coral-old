/*******************************************************************************
** Reflection code generated for type 'co.InterfaceTypeComponent'
**
** Created: Thu Jun 10 20:53:30 2010
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

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceTypeComponent_getSize();
co::Component* __InterfaceTypeComponent_newInstance();

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

	co::int32 getSize() { return __InterfaceTypeComponent_getSize(); }
	co::Component* newInstance() { return __InterfaceTypeComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createInterfaceTypeComponentReflector()
{
    return new InterfaceTypeComponent_Reflector;
}

} // namespace __coral
