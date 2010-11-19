/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeCreationTransactionComponent_getSize();
co::Component* __TypeCreationTransactionComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class TypeCreationTransactionComponent_Reflector : public co::ReflectorBase
{
public:
	TypeCreationTransactionComponent_Reflector()
	{
		// empty
	}

	virtual ~TypeCreationTransactionComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.TypeCreationTransactionComponent" );
	}

	co::int32 getSize()
	{
		return __TypeCreationTransactionComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __TypeCreationTransactionComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeCreationTransactionComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeCreationTransactionComponentReflector()
{
    return new TypeCreationTransactionComponent_Reflector;
}

} // namespace co
