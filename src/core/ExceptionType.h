/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _EXCEPTIONTYPE_H_
#define _EXCEPTIONTYPE_H_

#include "Type.h"
#include "ExceptionTypeComponent_Base.h"

namespace co {

/*!
	Component that implements co.ExceptionType.
 */
class ExceptionTypeComponent : public ExceptionTypeComponent_Base, public TypeImpl
{
public:
	virtual ~ExceptionTypeComponent();

	DELEGATE_CO_TYPE_METHODS( TypeImpl:: );
};

} // namespace co

#endif
