/*******************************************************************************
** Base class generated for component 'co.TypeCreationTransactionComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include "TypeCreationTransactionComponent_Base.h"
#include <co/Coral.h>
#include <co/ComponentType.h>
#include <co/InterfaceInfo.h>
#include <co/InterfaceType.h>

// ------ co.TypeCreationTransactionComponent provides an interface named 'transaction', of type co.TypeCreationTransaction ------ //

co::InterfaceType* co::TypeCreationTransactionComponent_co_TypeCreationTransaction::getInterfaceType()
{
	return co::typeOf<co::TypeCreationTransaction>::get();
}

const std::string& co::TypeCreationTransactionComponent_co_TypeCreationTransaction::getInterfaceName()
{
	static const std::string s_interfaceName( "transaction" );
	return s_interfaceName;
}

// ------ TypeCreationTransactionComponent_Base ------ //

co::TypeCreationTransactionComponent_Base::TypeCreationTransactionComponent_Base()
{
	// empty
}

co::TypeCreationTransactionComponent_Base::~TypeCreationTransactionComponent_Base()
{
	// empty
}

co::Component* co::TypeCreationTransactionComponent_Base::getInterfaceOwner()
{
	return this;
}

void co::TypeCreationTransactionComponent_Base::componentRetain()
{
	incrementRefCount();
}

void co::TypeCreationTransactionComponent_Base::componentRelease()
{
	decrementRefCount();
}

co::ComponentType* co::TypeCreationTransactionComponent_Base::getComponentType()
{
	co::Type* type = co::getType( "co.TypeCreationTransactionComponent" );
	assert( dynamic_cast<co::ComponentType*>( type ) );
	return static_cast<co::ComponentType*>( type );
}

co::Interface* co::TypeCreationTransactionComponent_Base::getInterface( co::InterfaceInfo* interfaceInfo )
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

void co::TypeCreationTransactionComponent_Base::bindInterface( co::InterfaceInfo* clientInterface, co::Interface* instance )
{
	checkValidClientInterface( clientInterface );
	raiseUnexpectedInterfaceIndex();
	CORAL_UNUSED( instance );
}
