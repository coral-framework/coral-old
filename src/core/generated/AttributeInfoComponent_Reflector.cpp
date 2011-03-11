/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __AttributeInfoComponent_getSize();
co::Component* __AttributeInfoComponent_newInstance();

// ------ Reflector ------ //

class AttributeInfoComponent_Reflector : public co::ReflectorBase
{
public:
	AttributeInfoComponent_Reflector()
	{
		// empty
	}

	virtual ~AttributeInfoComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.AttributeInfoComponent" );
	}

	co::int32 getSize()
	{
		return __AttributeInfoComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __AttributeInfoComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.AttributeInfoComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createAttributeInfoComponentReflector()
{
    return new AttributeInfoComponent_Reflector;
}

} // namespace co
