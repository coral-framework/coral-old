/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <lua/Exception.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ IReflector ------ //

class Exception_Reflector : public co::ReflectorBase
{
public:
	Exception_Reflector()
	{
		moduleRetain();
	}

	virtual ~Exception_Reflector()
	{
		moduleRelease();
	}

	co::IType* getType()
	{
		return co::typeOf<lua::Exception>::get();
	}

	co::int32 getSize()
	{
		return sizeof(lua::Exception);
	}

	void raise( const std::string& message )
	{
		throw lua::Exception( message );
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createExceptionIReflector()
{
    return new Exception_Reflector;
}

} // namespace lua
