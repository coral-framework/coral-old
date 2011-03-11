/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACETYPE_H_
#define _INTERFACETYPE_H_

#include "Type.h"
#include "AttributeAndMethodContainer.h"
#include "InterfaceTypeComponent_Base.h"

namespace co {

/*!
	Component that implements co.InterfaceType.
 */
class InterfaceTypeComponent : public InterfaceTypeComponent_Base,
						public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~InterfaceTypeComponent();

	// internal methods:
	void addSuperInterface( InterfaceType* superItf );
	void addSubInterface( InterfaceType* subItf );

	// InterfaceType methods:
	ArrayRange<InterfaceType* const> getInterfaceAncestors();
	ArrayRange<InterfaceType* const> getSuperInterfaces();
	ArrayRange<InterfaceType* const> getSubInterfaces();
	const std::string& getCppBlock();
	bool isSubTypeOf( InterfaceType* itf );

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_METHODCONTAINER_METHODS( AttributeAndMethodContainer:: );

private:
	ArrayRange<CompoundType* const> getCompoundTypeAncestors();
	
	// ancestors are computed lazily
	void updateAncestors();

private:
	typedef std::vector<InterfaceType*> InterfaceVector;
	InterfaceVector _ancestors;
	InterfaceVector _superInterfaces;
	InterfaceVector _subInterfaces;
};

} // namespace co

#endif
