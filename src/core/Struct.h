/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_STRUCT_H_
#define _CO_STRUCT_H_

#include "ClassType.h"
#include "Struct_Base.h"

namespace co {

/*!
	Implements co.IStruct.
 */
class Struct : public Struct_Base, public ClassTypeImpl
{
public:
	virtual ~Struct();

	DELEGATE_co_IType( ClassTypeImpl:: );
	DELEGATE_co_ICompositeType( ClassTypeImpl:: );
	DELEGATE_co_IRecordType( ClassTypeImpl:: );
};

} // namespace co

#endif
