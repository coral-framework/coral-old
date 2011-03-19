/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "MethodInfo.h"

namespace co {

MethodInfo::~MethodInfo()
{
	// empty
}

void MethodInfo::setReturnType( IType* returnType )
{
	_returnType = returnType;
}

void MethodInfo::setParameters( ArrayRange<IParameterInfo* const> parameters )
{
	assert( _parameters.size() == 0 );
	_parameters.reserve( parameters.getSize() );
	for( ; parameters; parameters.popFirst() )
		_parameters.push_back( parameters.getFirst() );
}

void MethodInfo::setExceptions( ArrayRange<IExceptionType* const> exceptions )
{
	assert( _exceptions.size() == 0 );
	_exceptions.reserve( exceptions.getSize() );
	for( ; exceptions; exceptions.popFirst() )
		_exceptions.push_back( exceptions.getFirst() );
}

IType* MethodInfo::getReturnType()
{
	return _returnType;
}

ArrayRange<IParameterInfo* const> MethodInfo::getParameters()
{
	return _parameters;
}

ArrayRange<IExceptionType* const> MethodInfo::getExceptions()
{
	return _exceptions;
}

CORAL_EXPORT_COMPONENT( MethodInfo, MethodInfo );

} // namespace co
