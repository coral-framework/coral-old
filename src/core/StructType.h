/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _STRUCTTYPE_H_
#define _STRUCTTYPE_H_

#include "Type.h"
#include "StructTypeComponent_Base.h"
#include "AttributeAndMethodContainer.h"

namespace co {

/*!
	Component that implements co.StructType.
 */
class StructTypeComponent : public StructTypeComponent_Base,
					public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~StructTypeComponent();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
};

} // namespace co

#endif
