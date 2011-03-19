/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __MethodBuilder_getSize();
co::IComponent* __MethodBuilder_newInstance();

// ------ IReflector ------ //

class MethodBuilder_Reflector : public co::ReflectorBase
{
public:
	MethodBuilder_Reflector()
	{
		// empty
	}

	virtual ~MethodBuilder_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.MethodBuilder" );
	}

	co::int32 getSize()
	{
		return __MethodBuilder_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __MethodBuilder_newInstance();
		assert( component->getComponentType()->getFullName() == "co.MethodBuilder" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createMethodBuilderIReflector()
{
    return new MethodBuilder_Reflector;
}

} // namespace co
