/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACETYPE_H_
#define _INTERFACETYPE_H_

#include "Type.h"
#include "AttributeAndMethodContainer.h"
#include "InterfaceTypeComponent_Base.h"

/*!
	Component that implements co.InterfaceType.
 */
class InterfaceType : public co::InterfaceTypeComponent_Base,
						public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~InterfaceType();

	// internal methods:
	void addSuperInterface( co::InterfaceType* superItf );
	void addSubInterface( co::InterfaceType* subItf );

	// co::InterfaceType methods:
	co::ArrayRange<co::InterfaceType* const> getInterfaceAncestors();
	co::ArrayRange<co::InterfaceType* const> getSuperInterfaces();
	co::ArrayRange<co::InterfaceType* const> getSubInterfaces();
	const std::string& getCppBlock();
	bool isSubTypeOf( co::InterfaceType* itf );

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_METHODCONTAINER_METHODS( AttributeAndMethodContainer:: );

private:
	co::ArrayRange<co::CompoundType* const> getCompoundTypeAncestors();
	
	// ancestors are computed lazily
	void updateAncestors();

private:
	typedef std::vector<co::InterfaceType*> InterfaceVector;
	InterfaceVector _ancestors;
	InterfaceVector _superInterfaces;
	InterfaceVector _subInterfaces;
};

#endif
