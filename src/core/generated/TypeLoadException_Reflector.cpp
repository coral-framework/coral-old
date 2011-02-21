/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/TypeLoadException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class TypeLoadException_Reflector : public co::ReflectorBase
{
public:
	TypeLoadException_Reflector()
	{
		// empty
	}

	virtual ~TypeLoadException_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::TypeLoadException>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeLoadException);
	}

	void raise( const std::string& message )
	{
		throw co::TypeLoadException( message );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeLoadExceptionReflector()
{
    return new TypeLoadException_Reflector;
}

} // namespace co
