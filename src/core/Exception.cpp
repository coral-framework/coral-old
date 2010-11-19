/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "Exception.h"

namespace co {

Exception::~Exception() throw()
{
	// empty
}

const char* Exception::what() const throw()
{
	return _message.c_str();
}

} // namespace co
