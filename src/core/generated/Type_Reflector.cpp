/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Type_getSize();
co::IObject* __Type_newInstance();

// ------ Reflector Component ------ //

class Type_Reflector : public co::ReflectorBase
{
public:
	Type_Reflector()
	{
		// empty
	}

	virtual ~Type_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Type" );
	}

	co::int32 getSize()
	{
		return __Type_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* component = __Type_newInstance();
		assert( component->getComponentType()->getFullName() == "co.Type" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createTypeReflector()
{
    return new Type_Reflector;
}

} // namespace co
