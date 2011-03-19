/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __StructType_getSize();
co::IComponent* __StructType_newInstance();

// ------ IReflector ------ //

class StructType_Reflector : public co::ReflectorBase
{
public:
	StructType_Reflector()
	{
		// empty
	}

	virtual ~StructType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.StructType" );
	}

	co::int32 getSize()
	{
		return __StructType_getSize();
	}

	co::IComponent* newInstance()
	{
		co::IComponent* component = __StructType_newInstance();
		assert( component->getComponentType()->getFullName() == "co.StructType" );
		return component;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createStructTypeIReflector()
{
    return new StructType_Reflector;
}

} // namespace co
