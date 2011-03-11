/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _METHODBUILDER_H_
#define _METHODBUILDER_H_

#include "MethodBuilderComponent_Base.h"
#include <co/RefVector.h>
#include <vector>

namespace co {

class Type;
class MethodInfo;
class TypeBuilder;
class ExceptionType;
class ParameterInfo;

/*!
	Component that implements co.MethodBuilder.
 */
class MethodBuilderComponent : public MethodBuilderComponent_Base
{
public:
	MethodBuilderComponent();
	virtual ~MethodBuilderComponent();

	// internal methods:
	void init( TypeBuilder* typeBuilder, const std::string& name );

	// MethodBuilder methods:
	TypeBuilder* getTypeBuilder();
	const std::string& getMethodName();
	void defineReturnType( Type* type );
	void defineParameter( const std::string& name, Type* type, bool input, bool output );
	void defineException( ExceptionType* exception );
	void createMethod();

private:
	std::string _name;
	RefPtr<TypeBuilder> _typeBuilder;
	RefPtr<MethodInfo> _createdMethodInfo;

	Type* _returnType;
	RefVector<ParameterInfo> _parameters;
	std::vector<ExceptionType*> _expectedExceptions;
};

} // namespace co

#endif
