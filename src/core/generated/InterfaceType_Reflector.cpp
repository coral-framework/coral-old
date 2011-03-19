/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceType_getSize();
co::IComponent* __InterfaceType_newInstance();

// ------ IReflector ------ //

class InterfaceType_Reflector : public co::ReflectorBase
{
public:
	InterfaceType_Reflector()
	{
		// empty
	}

	virtual ~InterfaceType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.InterfaceType" );
	}

	co::int32 getSize()
	{
		return __InterfaceType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __InterfaceType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.InterfaceType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createInterfaceTypeIReflector()
{
    return new InterfaceType_Reflector;
}

} // namespace co
