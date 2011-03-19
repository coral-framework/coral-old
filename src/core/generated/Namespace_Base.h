/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_NAMESPACE_BASE_H_
#define _CO_NAMESPACE_BASE_H_

#include <co/INamespace.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Namespace has a facet named 'namespace', of type co.INamespace.
class Namespace_co_INamespace : public co::INamespace
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Namespace'.
 */
class Namespace_Base : public co::ComponentBase,
	public Namespace_co_INamespace
{
public:
	Namespace_Base();
	virtual ~Namespace_Base();

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

#endif // _CO_NAMESPACE_BASE_H_
