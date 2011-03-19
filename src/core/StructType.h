/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _STRUCTTYPE_H_
#define _STRUCTTYPE_H_

#include "Type.h"
#include "StructType_Base.h"
#include "AttributeAndMethodContainer.h"

namespace co {

/*!
	Implements co.IStructType.
 */
class StructType : public StructType_Base,
					public TypeImpl, public AttributeAndMethodContainer
{
public:
	virtual ~StructType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
	DELEGATE_CO_COMPOUNDTYPE_METHODS( AttributeAndMethodContainer:: );
	DELEGATE_CO_ATTRIBUTECONTAINER_METHODS( AttributeAndMethodContainer:: );
};

} // namespace co

#endif
