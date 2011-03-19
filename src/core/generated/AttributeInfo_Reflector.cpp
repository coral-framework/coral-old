/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __AttributeInfo_getSize();
co::IComponent* __AttributeInfo_newInstance();

// ------ IReflector ------ //

class AttributeInfo_Reflector : public co::ReflectorBase
{
public:
	AttributeInfo_Reflector()
	{
		// empty
	}

	virtual ~AttributeInfo_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.AttributeInfo" );
	}

	co::int32 getSize()
	{
		return __AttributeInfo_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __AttributeInfo_newInstance();
		assert( component->getComponentType()->getFullName() == "co.AttributeInfo" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createAttributeInfoIReflector()
{
    return new AttributeInfo_Reflector;
}

} // namespace co
