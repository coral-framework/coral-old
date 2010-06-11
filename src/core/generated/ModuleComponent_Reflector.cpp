/*******************************************************************************
** Reflection code generated for type 'co.ModuleComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/Module.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModuleComponent_getSize();
co::Component* __ModuleComponent_newInstance();

class ModuleComponent_Reflector : public co::ReflectorBase
{
public:
	ModuleComponent_Reflector()
	{
		// empty
	}

	virtual ~ModuleComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ModuleComponent" );
	}

	co::int32 getSize() { return __ModuleComponent_getSize(); }
	co::Component* newInstance() { return __ModuleComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createModuleComponentReflector()
{
    return new ModuleComponent_Reflector;
}

} // namespace __coral
