/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Interface_getSize();
co::IObject* __Interface_newInstance();

// ------ Reflector Component ------ //

class Interface_Reflector : public co::ReflectorBase
{
public:
	Interface_Reflector()
	{
		// empty
	}

	virtual ~Interface_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Interface" );
	}

	co::int32 getSize()
	{
		return __Interface_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __Interface_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Interface" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createInterfaceReflector()
{
    return new Interface_Reflector;
}

} // namespace co
