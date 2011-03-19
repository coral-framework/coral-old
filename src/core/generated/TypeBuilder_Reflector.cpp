/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeBuilder_getSize();
co::IComponent* __TypeBuilder_newInstance();

// ------ IReflector ------ //

class TypeBuilder_Reflector : public co::ReflectorBase
{
public:
	TypeBuilder_Reflector()
	{
		// empty
	}

	virtual ~TypeBuilder_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.TypeBuilder" );
	}

	co::int32 getSize()
	{
		return __TypeBuilder_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __TypeBuilder_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeBuilder" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createTypeBuilderIReflector()
{
    return new TypeBuilder_Reflector;
}

} // namespace co
