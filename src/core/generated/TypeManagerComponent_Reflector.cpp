/*******************************************************************************
** Reflection code generated for type 'co.TypeManagerComponent'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/TypeManager.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Reflector ------ //

// Notice: the following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeManagerComponent_getSize();
co::Component* __TypeManagerComponent_newInstance();

class TypeManagerComponent_Reflector : public co::ReflectorBase
{
public:
	TypeManagerComponent_Reflector()
	{
		// empty
	}

	virtual ~TypeManagerComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.TypeManagerComponent" );
	}

	co::int32 getSize() { return __TypeManagerComponent_getSize(); }
	co::Component* newInstance() { return __TypeManagerComponent_newInstance(); }
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createTypeManagerComponentReflector()
{
    return new TypeManagerComponent_Reflector;
}

} // namespace __coral
