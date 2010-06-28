/*******************************************************************************
** Reflection code generated for type 'co.TypeKind'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/TypeKind.h>
#include <co/reserved/ReflectorBase.h>

// ------ Reflector ------ //

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

	co::Type* getType()
	{
		return co::typeOf<co::TypeKind>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeKind);
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createTypeKindReflector()
{
    return new TypeKind_Reflector;
}

} // namespace __coral
