/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _METHODBUILDER_H_
#define _METHODBUILDER_H_

#include "MethodBuilder_Base.h"
#include <co/RefVector.h>
#include <vector>

namespace co {

class IType;
class IMethodInfo;
class ITypeBuilder;
class IExceptionType;
class IParameterInfo;

/*!
	Implements co.IMethodBuilder.
 */
class MethodBuilder : public MethodBuilder_Base
{
public:
	MethodBuilder();
	virtual ~MethodBuilder();

	// internal methods:
	void init( ITypeBuilder* typeBuilder, const std::string& name );

	// IMethodBuilder methods:
	ITypeBuilder* getTypeBuilder();
	const std::string& getMethodName();
	void defineReturnType( IType* type );
	void defineParameter( const std::string& name, IType* type, bool input, bool output );
	void defineException( IExceptionType* exception );
	void createMethod();

private:
	std::string _name;
	RefPtr<ITypeBuilder> _typeBuilder;
	RefPtr<IMethodInfo> _createdMethodInfo;

	IType* _returnType;
	RefVector<IParameterInfo> _parameters;
	std::vector<IExceptionType*> _expectedExceptions;
};

} // namespace co

#endif
