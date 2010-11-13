/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _EXCEPTIONTYPE_H_
#define _EXCEPTIONTYPE_H_

#include "Type.h"
#include "ExceptionTypeComponent_Base.h"

/*!
	Component that implements co.ExceptionType.
 */
class ExceptionType : public co::ExceptionTypeComponent_Base, public TypeImpl
{
public:
	virtual ~ExceptionType();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
};

#endif
