/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __Field_getSize();
co::IObject* __Field_newInstance();

// ------ Reflector Component ------ //

class Field_Reflector : public co::ReflectorBase
{
public:
	Field_Reflector()
	{
		// empty
	}

	virtual ~Field_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.Field" );
	}

	co::int32 getSize()
	{
		return __Field_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __Field_newInstance();
		assert( instance->getComponent()->getFullName() == "co.Field" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createFieldReflector()
{
    return new Field_Reflector;
}

} // namespace co
