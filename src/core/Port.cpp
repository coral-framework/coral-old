/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Port.h"

namespace co {

Port::Port() : _type( 0 ), _isFacet( true )
{
	// empty
}


Port::~Port()
{
	// empty
}

void Port::setType( IInterface* interface )
{
	_type = interface;
}

void Port::setIsFacet( bool isFacet )
{
	_isFacet = isFacet;
}

MemberKind Port::getKind()
{
	return MK_PORT;
}

IInterface* Port::getType()
{
	return _type;
}

bool Port::getIsFacet()
{
	return _isFacet;
}

CORAL_EXPORT_COMPONENT( Port, Port );

} // namespace co
