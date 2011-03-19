/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _EXCEPTIONTYPE_H_
#define _EXCEPTIONTYPE_H_

#include "Type.h"
#include "ExceptionType_Base.h"

namespace co {

/*!
	Implements co.IExceptionType.
 */
class ExceptionType : public ExceptionType_Base, public TypeImpl
{
public:
	virtual ~ExceptionType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
};

} // namespace co

#endif
