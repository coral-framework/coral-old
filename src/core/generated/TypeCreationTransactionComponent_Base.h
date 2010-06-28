/*******************************************************************************
** Base class generated for component 'co.TypeCreationTransactionComponent'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_TYPECREATIONTRANSACTIONCOMPONENT_BASE_H_
#define _CO_TYPECREATIONTRANSACTIONCOMPONENT_BASE_H_

#include <co/reserved/ComponentBase.h>
#include <co/TypeCreationTransaction.h>

namespace co {

//! co.TypeCreationTransactionComponent provides an interface named 'transaction', of type co.TypeCreationTransaction
class TypeCreationTransactionComponent_co_TypeCreationTransaction : public co::TypeCreationTransaction
{
public:
	virtual co::InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeCreationTransactionComponent'.
 */
class TypeCreationTransactionComponent_Base : public co::ComponentBase,
	public TypeCreationTransactionComponent_co_TypeCreationTransaction
{
public:
	TypeCreationTransactionComponent_Base();
	virtual ~TypeCreationTransactionComponent_Base();

	// co::Interface Methods:
	virtual co::Component* getInterfaceOwner();
	virtual void componentRetain();
	virtual void componentRelease();

	// co::Component Methods:
	virtual co::ComponentType* getComponentType();
	virtual co::Interface* getInterface( co::InterfaceInfo* );
	virtual void bindInterface( co::InterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_TYPECREATIONTRANSACTIONCOMPONENT_BASE_H_
