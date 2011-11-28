/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Method.h"

namespace co {

Method::~Method()
{
	// empty
}

void Method::setReturnType( IType* returnType )
{
	_returnType = returnType;
}

void Method::setParameters( Range<IParameter* const> parameters )
{
	assert( _parameters.size() == 0 );
	_parameters.reserve( parameters.getSize() );
	for( ; parameters; parameters.popFirst() )
		_parameters.push_back( parameters.getFirst() );
}

void Method::setExceptions( Range<IException* const> exceptions )
{
	assert( _exceptions.size() == 0 );
	_exceptions.reserve( exceptions.getSize() );
	for( ; exceptions; exceptions.popFirst() )
		_exceptions.push_back( exceptions.getFirst() );
}

MemberKind Method::getKind()
{
	return MK_METHOD;
}

IType* Method::getReturnType()
{
	return _returnType;
}

Range<IParameter* const> Method::getParameters()
{
	return _parameters;
}

Range<IException* const> Method::getExceptions()
{
	return _exceptions;
}

CORAL_EXPORT_COMPONENT( Method, Method );

} // namespace co
