/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPEBUILDER_BASE_H_
#define _CO_TYPEBUILDER_BASE_H_

#include <co/ITypeBuilder.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.TypeBuilder has a facet named 'typeBuilder', of type co.ITypeBuilder.
class TypeBuilder_co_ITypeBuilder : public co::ITypeBuilder
{
public:
	virtual co::IInterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.TypeBuilder'.
 */
class TypeBuilder_Base : public co::ComponentBase,
	public TypeBuilder_co_ITypeBuilder
{
public:
	TypeBuilder_Base();
	virtual ~TypeBuilder_Base();

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

#endif // _CO_TYPEBUILDER_BASE_H_
