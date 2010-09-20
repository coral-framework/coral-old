/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceInfoComponent_getSize();
co::Component* __InterfaceInfoComponent_newInstance();

namespace co {

// ------ Reflector ------ //

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

	co::int32 getSize()
	{
		return __InterfaceInfoComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __InterfaceInfoComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.InterfaceInfoComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createInterfaceInfoComponentReflector()
{
    return new InterfaceInfoComponent_Reflector;
}

} // namespace co
