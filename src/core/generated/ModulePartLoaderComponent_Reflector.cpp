/*******************************************************************************
** Reflection code generated for type 'co.ModulePartLoaderComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/ModulePartLoader.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModulePartLoaderComponent_getSize();
co::Component* __ModulePartLoaderComponent_newInstance();

class ModulePartLoaderComponent_Reflector : public co::ReflectorBase
{
public:
	ModulePartLoaderComponent_Reflector()
	{
		// empty
	}

	virtual ~ModulePartLoaderComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ModulePartLoaderComponent" );
	}

	co::int32 getSize() { return __ModulePartLoaderComponent_getSize(); }
	co::Component* newInstance() { return __ModulePartLoaderComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createModulePartLoaderComponentReflector()
{
    return new ModulePartLoaderComponent_Reflector;
}

} // namespace __coral
