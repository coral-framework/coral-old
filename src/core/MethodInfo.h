/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _METHODINFO_H_
#define _METHODINFO_H_

#include "MemberInfo.h"
#include "MethodInfo_Base.h"
#include <co/RefVector.h>
#include <co/IExceptionType.h>
#include <co/IParameterInfo.h>

namespace co {

/*!
	Implements co.IMethodInfo.
 */
class MethodInfo : public MethodInfo_Base, public MemberInfoImpl
{
public:
	virtual ~MethodInfo();

	// internal methods:
	void setReturnType( IType* returnType );
	void setParameters( ArrayRange<IParameterInfo* const> parameters );
	void setExceptions( ArrayRange<IExceptionType* const> exceptions );

	// IMethodInfo methods:
	IType* getReturnType();
	ArrayRange<IParameterInfo* const> getParameters();
	ArrayRange<IExceptionType* const> getExceptions();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	IType* _returnType;

	typedef RefVector<IParameterInfo> ParameterVector;
	ParameterVector _parameters;

	typedef std::vector<IExceptionType*> ExceptionVector;
	ExceptionVector _exceptions;
};

} // namespace co

#endif
