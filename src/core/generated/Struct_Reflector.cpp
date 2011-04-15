/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __Struct_getSize();
co::IObject* __Struct_newInstance();

// ------ Reflector Component ------ //

class Struct_Reflector : public co::ReflectorBase
{
public:
	Struct_Reflector()
	{
		// empty
	}

	virtual ~Struct_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Struct" );
	}

	co::uint32 getSize()
	{
		return __Struct_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __Struct_newInstance();
		assert( instance->getComponent()->getFullName() == "co.Struct" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createStructReflector()
{
    return new Struct_Reflector;
}

} // namespace co
