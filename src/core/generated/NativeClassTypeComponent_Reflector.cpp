/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __NativeClassTypeComponent_getSize();
co::Component* __NativeClassTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class NativeClassTypeComponent_Reflector : public co::ReflectorBase
{
public:
	NativeClassTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~NativeClassTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.NativeClassTypeComponent" );
	}

	co::int32 getSize()
	{
		return __NativeClassTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __NativeClassTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.NativeClassTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createNativeClassTypeComponentReflector()
{
    return new NativeClassTypeComponent_Reflector;
}

} // namespace co
