/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeManager_getSize();
co::IComponent* __TypeManager_newInstance();

// ------ IReflector ------ //

class TypeManager_Reflector : public co::ReflectorBase
{
public:
	TypeManager_Reflector()
	{
		// empty
	}

	virtual ~TypeManager_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.TypeManager" );
	}

	co::int32 getSize()
	{
		return __TypeManager_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __TypeManager_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeManager" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createTypeManagerIReflector()
{
    return new TypeManager_Reflector;
}

} // namespace co
