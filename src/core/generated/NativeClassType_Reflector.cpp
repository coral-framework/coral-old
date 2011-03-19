/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __NativeClassType_getSize();
co::IComponent* __NativeClassType_newInstance();

// ------ IReflector ------ //

class NativeClassType_Reflector : public co::ReflectorBase
{
public:
	NativeClassType_Reflector()
	{
		// empty
	}

	virtual ~NativeClassType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.NativeClassType" );
	}

	co::int32 getSize()
	{
		return __NativeClassType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __NativeClassType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.NativeClassType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createNativeClassTypeIReflector()
{
    return new NativeClassType_Reflector;
}

} // namespace co
