/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_METHOD_H_
#define _CO_METHOD_H_

#include "Member.h"
#include "Method_Base.h"
#include <co/RefVector.h>
#include <co/IException.h>
#include <co/IParameter.h>

namespace co {

/*!
	Implements co.IMethod.
 */
class Method : public Method_Base, public MemberImpl
{
public:
	virtual ~Method();

	// internal methods:
	void setReturnType( IType* returnType );
	void setParameters( Range<IParameter* const> parameters );
	void setExceptions( Range<IException* const> exceptions );

	// IMember methods:
	MemberKind getKind();

	// IMethod methods:
	IType* getReturnType();
	Range<IParameter* const> getParameters();
	Range<IException* const> getExceptions();

	DELEGATE_co_IMember( MemberImpl:: );

private:
	IType* _returnType;

	typedef RefVector<IParameter> ParameterVector;
	ParameterVector _parameters;

	typedef std::vector<IException*> ExceptionVector;
	ExceptionVector _exceptions;
};

} // namespace co

#endif
