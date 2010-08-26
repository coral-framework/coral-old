/*******************************************************************************
** Reflection code generated for type 'co.ModulePartLoaderComponent'
**
** Created: Wed Aug 25 16:31:31 2010
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
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ModulePartLoaderComponent_getSize();
co::Component* __ModulePartLoaderComponent_newInstance();

namespace co {

// ------ Reflector ------ //

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

	co::int32 getSize()
	{
		return __ModulePartLoaderComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ModulePartLoaderComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ModulePartLoaderComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createModulePartLoaderComponentReflector()
{
    return new ModulePartLoaderComponent_Reflector;
}

} // namespace co
