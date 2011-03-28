/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/MemberKind.h>
#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

class MemberKind_Reflector : public co::ReflectorBase
{
public:
	MemberKind_Reflector()
	{
		// empty
	}

	virtual ~MemberKind_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::MemberKind>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MemberKind);
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createMemberKindReflector()
{
    return new MemberKind_Reflector;
}

} // namespace co
