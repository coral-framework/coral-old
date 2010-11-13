/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "MethodInfo.h"

MethodInfo::~MethodInfo()
{
	// empty
}

void MethodInfo::setReturnType( co::Type* returnType )
{
	_returnType = returnType;
}

void MethodInfo::setParameters( co::ArrayRange<co::ParameterInfo* const> parameters )
{
	assert( _parameters.size() == 0 );
	_parameters.reserve( parameters.getSize() );
	for( ; parameters; parameters.popFirst() )
		_parameters.push_back( parameters.getFirst() );
}

void MethodInfo::setExceptions( co::ArrayRange<co::ExceptionType* const> exceptions )
{
	assert( _exceptions.size() == 0 );
	_exceptions.reserve( exceptions.getSize() );
	for( ; exceptions; exceptions.popFirst() )
		_exceptions.push_back( exceptions.getFirst() );
}

co::Type* MethodInfo::getReturnType()
{
	return _returnType;
}

co::ArrayRange<co::ParameterInfo* const> MethodInfo::getParameters()
{
	return _parameters;
}

co::ArrayRange<co::ExceptionType* const> MethodInfo::getExceptions()
{
	return _exceptions;
}

CORAL_EXPORT_COMPONENT( MethodInfo, MethodInfoComponent );
