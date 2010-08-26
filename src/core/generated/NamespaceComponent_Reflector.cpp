/*******************************************************************************
** Reflection code generated for type 'co.NamespaceComponent'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/Namespace.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __NamespaceComponent_getSize();
co::Component* __NamespaceComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class NamespaceComponent_Reflector : public co::ReflectorBase
{
public:
	NamespaceComponent_Reflector()
	{
		// empty
	}

	virtual ~NamespaceComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.NamespaceComponent" );
	}

	co::int32 getSize()
	{
		return __NamespaceComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __NamespaceComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.NamespaceComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createNamespaceComponentReflector()
{
    return new NamespaceComponent_Reflector;
}

} // namespace co
