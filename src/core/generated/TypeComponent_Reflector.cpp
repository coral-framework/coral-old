/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeComponent_getSize();
co::Component* __TypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class TypeComponent_Reflector : public co::ReflectorBase
{
public:
	TypeComponent_Reflector()
	{
		// empty
	}

	virtual ~TypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.TypeComponent" );
	}

	co::int32 getSize()
	{
		return __TypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __TypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeComponentReflector()
{
    return new TypeComponent_Reflector;
}

} // namespace co
