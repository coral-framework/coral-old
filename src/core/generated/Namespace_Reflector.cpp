/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Namespace_getSize();
co::IComponent* __Namespace_newInstance();

// ------ IReflector ------ //

class Namespace_Reflector : public co::ReflectorBase
{
public:
	Namespace_Reflector()
	{
		// empty
	}

	virtual ~Namespace_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Namespace" );
	}

	co::int32 getSize()
	{
		return __Namespace_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __Namespace_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Namespace" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createNamespaceIReflector()
{
    return new Namespace_Reflector;
}

} // namespace co
