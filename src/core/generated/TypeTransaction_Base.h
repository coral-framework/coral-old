/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPETRANSACTION_BASE_H_
#define _CO_TYPETRANSACTION_BASE_H_

#include <co/ITypeTransaction.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeTransaction has a facet named 'transaction', of type co.ITypeTransaction.
class TypeTransaction_co_ITypeTransaction : public co::ITypeTransaction
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeTransaction'.
 */
class TypeTransaction_Base : public co::ComponentBase,
	public TypeTransaction_co_ITypeTransaction
{
public:
	TypeTransaction_Base();
	virtual ~TypeTransaction_Base();

	// co::IService Methods:
	co::IObject* getInterfaceOwner();
	void componentRetain();
	void componentRelease();

	// co::IObject Methods:
	co::IComponent* getComponentType();
	co::IService* getInterface( co::IPort* );
	void setReceptacle( co::IPort*, co::IService* );
};

} // namespace co

#endif // _CO_TYPETRANSACTION_BASE_H_
