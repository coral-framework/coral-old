/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace co {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __CppBlockAnnotation_getSize();
co::IObject* __CppBlockAnnotation_newInstance();

// ------ Reflector Component ------ //

class CppBlockAnnotation_Reflector : public co::ReflectorBase
{
public:
	CppBlockAnnotation_Reflector()
	{
		// empty
	}

	virtual ~CppBlockAnnotation_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::getType( "co.CppBlockAnnotation" );
	}

	co::uint32 getSize()
	{
		return __CppBlockAnnotation_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __CppBlockAnnotation_newInstance();
		assert( instance->getComponent()->getFullName() == "co.CppBlockAnnotation" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createCppBlockAnnotationReflector()
{
    return new CppBlockAnnotation_Reflector;
}

} // namespace co
