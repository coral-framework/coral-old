/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_FIELD_BASE_H_
#define _CO_FIELD_BASE_H_

#include <co/IField.h>
#include <co/reserved/ComponentBase.h>

namespace co {

//! co.Field has a facet named 'attributeInfo', of type co.IField.
class Field_co_IField : public co::IField
{
public:
	virtual co::IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement the component 'co.Field'.
 */
class Field_Base : public co::ComponentBase,
	public Field_co_IField
{
public:
	Field_Base();
	virtual ~Field_Base();

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

#endif // _CO_FIELD_BASE_H_
