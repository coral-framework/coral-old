/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _STRUCTTYPE_H_
#define _STRUCTTYPE_H_

#include "Type.h"
#include "StructTypeComponent_Base.h"
#include "AttributeAndMethodContainer.h"

/*!
	Component that implements co.StructType.
 */
class StructType : public co::StructTypeComponent_Base,
					public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~StructType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
};

#endif
