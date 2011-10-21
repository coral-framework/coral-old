/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/reserved/ReflectorBase.h>
#include <cassert>

namespace lua {

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::uint32 __ScriptedAnnotation_getSize();
co::IObject* __ScriptedAnnotation_newInstance();

void moduleRetain();
void moduleRelease();

// ------ Reflector Component ------ //

class ScriptedAnnotation_Reflector : public co::ReflectorBase
{
public:
	ScriptedAnnotation_Reflector()
	{
		moduleRetain();
	}

	virtual ~ScriptedAnnotation_Reflector()
	{
		moduleRelease();
	}

	co::IType* getType()
	{
		return co::getType( "lua.ScriptedAnnotation" );
	}

	co::uint32 getSize()
	{
		return __ScriptedAnnotation_getSize();
	}

	co::IObject* newInstance()
	{
		co::IObject* instance = __ScriptedAnnotation_newInstance();
		assert( instance->getComponent()->getFullName() == "lua.ScriptedAnnotation" );
		return instance;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createScriptedAnnotationReflector()
{
    return new ScriptedAnnotation_Reflector;
}

} // namespace lua
