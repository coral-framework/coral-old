/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _EXCEPTIONTYPE_H_
#define _EXCEPTIONTYPE_H_

#include "Type.h"
#include "ExceptionType_Base.h"

namespace co {

/*!
	Implements co.IException.
 */
class ExceptionType : public ExceptionType_Base, public TypeImpl
{
public:
	virtual ~ExceptionType();

	DELEGATE_co_IType( TypeImpl:: );
};

} // namespace co

#endif
