/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPECREATIONTRANSACTION_BASE_H_
#define _CO_TYPECREATIONTRANSACTION_BASE_H_

#include <co/ITypeCreationTransaction.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeCreationTransaction has a facet named 'transaction', of type co.ITypeCreationTransaction.
class TypeCreationTransaction_co_ITypeCreationTransaction : public co::ITypeCreationTransaction
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeCreationTransaction'.
 */
class TypeCreationTransaction_Base : public co::ComponentBase,
	public TypeCreationTransaction_co_ITypeCreationTransaction
{
public:
	TypeCreationTransaction_Base();
	virtual ~TypeCreationTransaction_Base();

	// co::Interface Methods:
	co::IComponent* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IComponent Methods:
	co::IComponentType* getComponentType();
	co::Interface* getInterface( co::IInterfaceInfo* );
	void setReceptacle( co::IInterfaceInfo*, co::Interface* );
};

} // namespace co

#endif // _CO_TYPECREATIONTRANSACTION_BASE_H_
