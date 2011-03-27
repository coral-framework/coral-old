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
	virtual co::IInterface* getInterfaceType();
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

#endif // _CO_TYPEBUILDER_BASE_H_
