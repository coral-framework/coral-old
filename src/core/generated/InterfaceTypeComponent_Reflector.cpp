/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceTypeComponent_getSize();
co::Component* __InterfaceTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class InterfaceTypeComponent_Reflector : public co::ReflectorBase
{
public:
	InterfaceTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~InterfaceTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.InterfaceTypeComponent" );
	}

	co::int32 getSize()
	{
		return __InterfaceTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __InterfaceTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.InterfaceTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createInterfaceTypeComponentReflector()
{
    return new InterfaceTypeComponent_Reflector;
}

} // namespace co
