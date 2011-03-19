/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __EnumType_getSize();
co::IComponent* __EnumType_newInstance();

// ------ IReflector ------ //

class EnumType_Reflector : public co::ReflectorBase
{
public:
	EnumType_Reflector()
	{
		// empty
	}

	virtual ~EnumType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.EnumType" );
	}

	co::int32 getSize()
	{
		return __EnumType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __EnumType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.EnumType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createEnumTypeIReflector()
{
    return new EnumType_Reflector;
}

} // namespace co
