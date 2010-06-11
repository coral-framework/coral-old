/*
 * Coral - A C++ Component Framework.
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
}

/*!
	Component that implements co.MethodBuilder.
 */
class MethodBuilder : public co::MethodBuilderComponent_Base
{
public:
	MethodBuilder();
	virtual ~MethodBuilder();

	// internal methods:
	void init( co::TypeBuilder* typeBuilder, const std::string& name );

	// co::MethodBuilder methods:
	co::TypeBuilder* getTypeBuilder();
	const std::string& getMethodName();
	void defineReturnType( co::Type* type );
	void defineParameter( const std::string& name, co::Type* type, bool input, bool output );
	void defineException( co::ExceptionType* exception );
	void createMethod();

private:
	std::string _name;
	co::RefPtr<co::TypeBuilder> _typeBuilder;
	co::RefPtr<co::MethodInfo> _createdMethodInfo;

	co::Type* _returnType;
	co::RefVector<co::ParameterInfo> _parameters;
	std::vector<co::ExceptionType*> _expectedExceptions;
};

#endif
