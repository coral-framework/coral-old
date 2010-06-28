/*******************************************************************************
** Reflection code generated for type 'co.ModuleManagerComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ModuleManager.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModuleManagerComponent_getSize();
co::Component* __ModuleManagerComponent_newInstance();

class ModuleManagerComponent_Reflector : public co::ReflectorBase
{
public:
	ModuleManagerComponent_Reflector()
	{
		// empty
	}

	virtual ~ModuleManagerComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ModuleManagerComponent" );
	}

	co::int32 getSize() { return __ModuleManagerComponent_getSize(); }
	co::Component* newInstance() { return __ModuleManagerComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createModuleManagerComponentReflector()
{
    return new ModuleManagerComponent_Reflector;
}

} // namespace __coral
