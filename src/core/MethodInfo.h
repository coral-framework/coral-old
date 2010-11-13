/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _METHODINFO_H_
#define _METHODINFO_H_

#include "MemberInfo.h"
#include "MethodInfoComponent_Base.h"
#include <co/RefVector.h>
#include <co/ExceptionType.h>
#include <co/ParameterInfo.h>

/*!
	Component that implements co.MethodInfo.
 */
class MethodInfo : public co::MethodInfoComponent_Base, public MemberInfoImpl
{
public:
	virtual ~MethodInfo();

	// internal methods:
	void setReturnType( co::Type* returnType );
	void setParameters( co::ArrayRange<co::ParameterInfo* const> parameters );
	void setExceptions( co::ArrayRange<co::ExceptionType* const> exceptions );

	// co::MethodInfo methods:
	co::Type* getReturnType();
	co::ArrayRange<co::ParameterInfo* const> getParameters();
	co::ArrayRange<co::ExceptionType* const> getExceptions();

	DELEGATE_CO_MEMBERINFO_METHODS( MemberInfoImpl:: );

private:
	co::Type* _returnType;

	typedef co::RefVector<co::ParameterInfo> ParameterVector;
	ParameterVector _parameters;

	typedef std::vector<co::ExceptionType*> ExceptionVector;
	ExceptionVector _exceptions;
};

#endif
