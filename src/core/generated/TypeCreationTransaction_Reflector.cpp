/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeCreationTransaction_getSize();
co::IComponent* __TypeCreationTransaction_newInstance();

// ------ IReflector ------ //

class TypeCreationTransaction_Reflector : public co::ReflectorBase
{
public:
	TypeCreationTransaction_Reflector()
	{
		// empty
	}

	virtual ~TypeCreationTransaction_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.TypeCreationTransaction" );
	}

	co::int32 getSize()
	{
		return __TypeCreationTransaction_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __TypeCreationTransaction_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeCreationTransaction" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createTypeCreationTransactionIReflector()
{
    return new TypeCreationTransaction_Reflector;
}

} // namespace co
