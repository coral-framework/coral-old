/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _INTERFACETYPE_H_
#define _INTERFACETYPE_H_

#include "Type.h"
#include "AttributeAndMethodContainer.h"
#include "InterfaceType_Base.h"

namespace co {

/*!
	Implements co.IInterfaceType.
 */
class InterfaceType : public InterfaceType_Base,
						public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~InterfaceType();

	// internal methods:
	void addSuperInterface( IInterfaceType* superItf );
	void addSubInterface( IInterfaceType* subItf );

	// IInterfaceType methods:
	ArrayRange<IInterfaceType* const> getInterfaceAncestors();
	ArrayRange<IInterfaceType* const> getSuperInterfaces();
	ArrayRange<IInterfaceType* const> getSubInterfaces();
	const std::string& getCppBlock();
	bool isSubTypeOf( IInterfaceType* itf );

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_METHODCONTAINER_METHODS( AttributeAndMethodContainer:: );

private:
	ArrayRange<ICompoundType* const> getCompoundTypeAncestors();

	// ancestors are computed lazily
	void updateAncestors();

private:
	typedef std::vector<IInterfaceType*> InterfaceVector;
	InterfaceVector _ancestors;
	InterfaceVector _superInterfaces;
	InterfaceVector _subInterfaces;
};

} // namespace co

#endif
