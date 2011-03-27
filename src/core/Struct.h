/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_STRUCT_H_
#define _CO_STRUCT_H_

#include "Struct_Base.h"
#include "CompositeType.h"

namespace co {

/*!
	Implements co.IStruct.
 */
class Struct : public Struct_Base, public CompositeTypeImpl
{
public:
	virtual ~Struct();

	DELEGATE_co_IType( CompositeTypeImpl:: );
	DELEGATE_co_ICompositeType( CompositeTypeImpl:: );
	DELEGATE_co_IRecordType( CompositeTypeImpl:: );
};

} // namespace co

#endif
