/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _METHODINFO_H_
#define _METHODINFO_H_

#include "MemberInfo.h"
#include "MethodInfoComponent_Base.h"
#include <co/RefVector.h>
#include <co/ExceptionType.h>
#include <co/ParameterInfo.h>

namespace co {

/*!
	Component that implements co.MethodInfo.
 */
class MethodInfoComponent : public MethodInfoComponent_Base, public MemberInfoImpl
{
public:
	virtual ~MethodInfoComponent();

	// internal methods:
	void setReturnType( Type* returnType );
	void setParameters( ArrayRange<ParameterInfo* const> parameters );
	void setExceptions( ArrayRange<ExceptionType* const> exceptions );

	// MethodInfo methods:
	Type* getReturnType();
	ArrayRange<ParameterInfo* const> getParameters();
	ArrayRange<ExceptionType* const> getExceptions();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	Type* _returnType;

	typedef RefVector<ParameterInfo> ParameterVector;
	ParameterVector _parameters;

	typedef std::vector<ExceptionType*> ExceptionVector;
	ExceptionVector _exceptions;
};

} // namespace co

#endif
