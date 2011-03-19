/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __ArrayType_getSize();
co::IComponent* __ArrayType_newInstance();

// ------ IReflector ------ //

class ArrayType_Reflector : public co::ReflectorBase
{
public:
	ArrayType_Reflector()
	{
		// empty
	}

	virtual ~ArrayType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.ArrayType" );
	}

	co::int32 getSize()
	{
		return __ArrayType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __ArrayType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.ArrayType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createArrayTypeIReflector()
{
    return new ArrayType_Reflector;
}

} // namespace co
