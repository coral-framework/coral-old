/*******************************************************************************
** Reflection code generated for type 'co.NamespaceComponent'
**
** Created: Mon Jun 28 12:01:36 2010
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

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __NamespaceComponent_getSize();
co::Component* __NamespaceComponent_newInstance();

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

	co::int32 getSize() { return __NamespaceComponent_getSize(); }
	co::Component* newInstance() { return __NamespaceComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createNamespaceComponentReflector()
{
    return new NamespaceComponent_Reflector;
}

} // namespace __coral
