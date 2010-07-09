/*******************************************************************************
** Base class generated for component 'co.TypeCreationTransactionComponent'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "TypeCreationTransactionComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

namespace co {

// ------ co.TypeCreationTransactionComponent provides an interface named 'transaction', of type co.TypeCreationTransaction ------ //

co::InterfaceType* TypeCreationTransactionComponent_co_TypeCreationTransaction::getInterfaceType()
{
	return co::typeOf<co::TypeCreationTransaction>::get();
}

const std::string& TypeCreationTransactionComponent_co_TypeCreationTransaction::getInterfaceName()
{
	static const std::string s_interfaceName( "transaction" );
	return s_interfaceName;
}

// ------ TypeCreationTransactionComponent_Base ------ //

TypeCreationTransactionComponent_Base::TypeCreationTransactionComponent_Base()
{
	// empty
}

TypeCreationTransactionComponent_Base::~TypeCreationTransactionComponent_Base()
{
	// empty
}

co::Component* TypeCreationTransactionComponent_Base::getInterfaceOwner()
{
	return this;
}

void TypeCreationTransactionComponent_Base::componentRetain()
{
	incrementRefCount();
}

void TypeCreationTransactionComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* TypeCreationTransactionComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeCreationTransactionComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* TypeCreationTransactionComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
{
	checkValidInterface( interfaceInfo );
	co::Interface* res = NULL;
	switch( interfaceInfo->getIndex() )
	{
	case 0:		res = co::disambiguate<co::Interface, co::TypeCreationTransaction>( this ); break;
	default:	raiseUnexpectedInterfaceIndex();
	}
	return res;
}

void TypeCreationTransactionComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}

} // namespace co
