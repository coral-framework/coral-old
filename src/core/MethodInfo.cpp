/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodInfo.h"

namespace co {

MethodInfoComponent::~MethodInfoComponent()
{
	// empty
}

void MethodInfoComponent::setReturnType( Type* returnType )
{
	_returnType = returnType;
}

void MethodInfoComponent::setParameters( ArrayRange<ParameterInfo* const> parameters )
{
	assert( _parameters.size() == 0 );
	_parameters.reserve( parameters.getSize() );
	for( ; parameters; parameters.popFirst() )
		_parameters.push_back( parameters.getFirst() );
}

void MethodInfoComponent::setExceptions( ArrayRange<ExceptionType* const> exceptions )
{
	assert( _exceptions.size() == 0 );
	_exceptions.reserve( exceptions.getSize() );
	for( ; exceptions; exceptions.popFirst() )
		_exceptions.push_back( exceptions.getFirst() );
}

Type* MethodInfoComponent::getReturnType()
{
	return _returnType;
}

ArrayRange<ParameterInfo* const> MethodInfoComponent::getParameters()
{
	return _parameters;
}

ArrayRange<ExceptionType* const> MethodInfoComponent::getExceptions()
{
	return _exceptions;
}

CORAL_EXPORT_COMPONENT( MethodInfoComponent, MethodInfoComponent );

} // namespace co
