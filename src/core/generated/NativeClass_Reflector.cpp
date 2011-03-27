/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __NativeClass_getSize();
co::IObject* __NativeClass_newInstance();

// ------ Reflector Component ------ //

class NativeClass_Reflector : public co::ReflectorBase
{
public:
	NativeClass_Reflector()
	{
		// empty
	}

	virtual ~NativeClass_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.NativeClass" );
	}

	co::int32 getSize()
	{
		return __NativeClass_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __NativeClass_newInstance();
		assert( component->getComponentType()->getFullName() == "co.NativeClass" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createNativeClassReflector()
{
    return new NativeClass_Reflector;
}

} // namespace co
