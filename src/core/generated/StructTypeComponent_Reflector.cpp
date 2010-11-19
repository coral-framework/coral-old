/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __StructTypeComponent_getSize();
co::Component* __StructTypeComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class StructTypeComponent_Reflector : public co::ReflectorBase
{
public:
	StructTypeComponent_Reflector()
	{
		// empty
	}

	virtual ~StructTypeComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.StructTypeComponent" );
	}

	co::int32 getSize()
	{
		return __StructTypeComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __StructTypeComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.StructTypeComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createStructTypeComponentReflector()
{
    return new StructTypeComponent_Reflector;
}

} // namespace co
