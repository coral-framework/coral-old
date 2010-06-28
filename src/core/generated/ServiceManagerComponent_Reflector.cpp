/*******************************************************************************
** Reflection code generated for type 'co.ServiceManagerComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ServiceManager.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ServiceManagerComponent_getSize();
co::Component* __ServiceManagerComponent_newInstance();

class ServiceManagerComponent_Reflector : public co::ReflectorBase
{
public:
	ServiceManagerComponent_Reflector()
	{
		// empty
	}

	virtual ~ServiceManagerComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ServiceManagerComponent" );
	}

	co::int32 getSize() { return __ServiceManagerComponent_getSize(); }
	co::Component* newInstance() { return __ServiceManagerComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createServiceManagerComponentReflector()
{
    return new ServiceManagerComponent_Reflector;
}

} // namespace __coral
