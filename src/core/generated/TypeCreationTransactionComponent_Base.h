/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPECREATIONTRANSACTIONCOMPONENT_BASE_H_
#define _CO_TYPECREATIONTRANSACTIONCOMPONENT_BASE_H_

#include <co/TypeCreationTransaction.h>
#include <co/reserved/ComponentBase.h>

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
