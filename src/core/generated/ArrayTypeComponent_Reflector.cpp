/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ArrayTypeComponent_getSize();
co::Component* __ArrayTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class ArrayTypeComponent_Reflector : public co::ReflectorBase
{
public:
	ArrayTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~ArrayTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.ArrayTypeComponent" );
	}

	co::int32 getSize()
	{
		return __ArrayTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __ArrayTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ArrayTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createArrayTypeComponentReflector()
{
    return new ArrayTypeComponent_Reflector;
}

} // namespace co
