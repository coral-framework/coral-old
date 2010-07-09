/*******************************************************************************
** Reflection code generated for type 'co.TypeCreationTransactionComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/reserved/ReflectorBase.h>
#include <co/TypeCreationTransaction.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

// The following two functions are implemented by CORAL_EXPORT_COMPONENT()
co::int32 __TypeCreationTransactionComponent_getSize();
co::Component* __TypeCreationTransactionComponent_newInstance();

namespace co {

// ------ Reflector ------ //

class TypeCreationTransactionComponent_Reflector : public co::ReflectorBase
{
public:
	TypeCreationTransactionComponent_Reflector()
	{
		// empty
	}

	virtual ~TypeCreationTransactionComponent_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::getType( "co.TypeCreationTransactionComponent" );
	}

	co::int32 getSize()
	{
		return __TypeCreationTransactionComponent_getSize();
	}

	co::Component* newInstance()
	{
		co::Component* component = __TypeCreationTransactionComponent_newInstance();
		assert( component->getComponentType()->getFullName() == "co.TypeCreationTransactionComponent" );
		return component;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeCreationTransactionComponentReflector()
{
    return new TypeCreationTransactionComponent_Reflector;
}

} // namespace co
