/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include "RefCounted.h"
#include <co/Coral.h>
#include <cassert>
#include <iostream>

namespace co {

RefCounted::~RefCounted()
{
	if( _refCount != 0 )
		debug( Dbg_Fatal, "Deleting object %p with a reference count of %i.", this, _refCount );
}

} // namespace co
