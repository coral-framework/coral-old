/*******************************************************************************
** Reflection code generated for type 'co.InterfaceInfoComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/InterfaceInfo.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceInfoComponent_getSize();
co::Component* __InterfaceInfoComponent_newInstance();

class InterfaceInfoComponent_Reflector : public co::ReflectorBase
{
public:
	InterfaceInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~InterfaceInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.InterfaceInfoComponent" );
	}

	co::int32 getSize() { return __InterfaceInfoComponent_getSize(); }
	co::Component* newInstance() { return __InterfaceInfoComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createInterfaceInfoComponentReflector()
{
    return new InterfaceInfoComponent_Reflector;
}

} // namespace __coral
