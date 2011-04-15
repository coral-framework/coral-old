/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __NativeClass_getSize();
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

	co::uint32 getSize()
	{
		return __NativeClass_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __NativeClass_newInstance();
		assert( instance->getComponent()->getFullName() == "co.NativeClass" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createNativeClassReflector()
{
    return new NativeClass_Reflector;
}

} // namespace co
