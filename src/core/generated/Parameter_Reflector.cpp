/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Parameter_getSize();
co::IObject* __Parameter_newInstance();

// ------ Reflector Component ------ //

class Parameter_Reflector : public co::ReflectorBase
{
public:
	Parameter_Reflector()
	{
		// empty
	}

	virtual ~Parameter_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Parameter" );
	}

	co::int32 getSize()
	{
		return __Parameter_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __Parameter_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Parameter" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createParameterReflector()
{
    return new Parameter_Reflector;
}

} // namespace co
