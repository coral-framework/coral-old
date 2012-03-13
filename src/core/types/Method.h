/*
 * Coral - Lightweight C++ Component Framework
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
class Method : public Member<Method_Base>
{
public:
	virtual ~Method();

	// internal methods:
	void setReturnType( IType* returnType );
	void setParameters( Range<IParameter*> parameters );
	void setExceptions( Range<IException*> exceptions );

	// IMember methods:
	MemberKind getKind();

	// IMethod methods:
	IType* getReturnType();
	Range<IParameter*> getParameters();
	Range<IException*> getExceptions();

private:
	IType* _returnType;

	typedef RefVector<IParameter> ParameterVector;
	ParameterVector _parameters;

	typedef std::vector<IException*> ExceptionVector;
	ExceptionVector _exceptions;
};

} // namespace co

#endif
