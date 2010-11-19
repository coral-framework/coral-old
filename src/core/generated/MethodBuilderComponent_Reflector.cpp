/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __MethodBuilderComponent_getSize();
co::Component* __MethodBuilderComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class MethodBuilderComponent_Reflector : public co::ReflectorBase
{
public:
	MethodBuilderComponent_Reflector()
	{
		// empty
	}

	virtual ~MethodBuilderComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.MethodBuilderComponent" );
	}

	co::int32 getSize()
	{
		return __MethodBuilderComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __MethodBuilderComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.MethodBuilderComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodBuilderComponentReflector()
{
    return new MethodBuilderComponent_Reflector;
}

} // namespace co
