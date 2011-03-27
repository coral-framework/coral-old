/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/TypeKind.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

class TypeKind_Reflector : public co::ReflectorBase
{
public:
	TypeKind_Reflector()
	{
		// empty
	}

	virtual ~TypeKind_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::TypeKind>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeKind);
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createTypeKindReflector()
{
    return new TypeKind_Reflector;
}

} // namespace co
