/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "BasicReflector.h"
#include <co/Any.h>
#include <co/Type.h>
#include <co/UnsupportedOperationException.h>

BasicReflector::BasicReflector( co::Type* type ) : _type( type )
{
	assert( _type );
}

BasicReflector::~BasicReflector()
{
	// empty
}

co::Type* BasicReflector::getType()
{
	return _type;
}

co::int32 BasicReflector::getSize()
{
	static const co::int32 s_typeSizes[] =
	{
		-1,							// TK_NONE
		sizeof(co::Any),			// TK_ANY
		1,							// TK_BOOLEAN
		1,							// TK_INT8
		1,							// TK_UINT8
		2,							// TK_INT16
		2,							// TK_UINT16
		4,							// TK_INT32
		4,							// TK_UINT32
		8,							// TK_INT64
		8,							// TK_UINT64
		4,							// TK_FLOAT
		8,							// TK_DOUBLE
		sizeof(std::string),		// TK_STRING
		sizeof(std::vector<int>),	// TK_ARRAY
		4,							// TK_ENUM
		sizeof(co::Exception)		// TK_EXCEPTION
	};

	co::TypeKind kind = _type->getKind();
	if( kind == co::TK_COMPONENT )
		return 0;

	assert( kind < co::TK_STRUCT );
	return s_typeSizes[kind];
}

co::Component* BasicReflector::newInstance()
{
	if( _type->getKind() == co::TK_COMPONENT )
		throw co::UnsupportedOperationException( "cannot instantiate an internal component" );

	return ReflectorBase::newInstance();
}
