/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
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
