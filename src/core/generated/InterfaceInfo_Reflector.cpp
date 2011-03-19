/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __InterfaceInfo_getSize();
co::IComponent* __InterfaceInfo_newInstance();

// ------ IReflector ------ //

class InterfaceInfo_Reflector : public co::ReflectorBase
{
public:
	InterfaceInfo_Reflector()
	{
		// empty
	}

	virtual ~InterfaceInfo_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.InterfaceInfo" );
	}

	co::int32 getSize()
	{
		return __InterfaceInfo_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __InterfaceInfo_newInstance();
		assert( component->getComponentType()->getFullName() == "co.InterfaceInfo" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createInterfaceInfoIReflector()
{
    return new InterfaceInfo_Reflector;
}

} // namespace co
