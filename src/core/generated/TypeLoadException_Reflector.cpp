/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/TypeLoadException.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::TypeLoadException>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(co::TypeLoadException);
	}

	void raise( const std::string& message )
	{
		throw co::TypeLoadException( message );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createTypeLoadExceptionReflector()
{
    return new TypeLoadException_Reflector;
}

} // namespace co
