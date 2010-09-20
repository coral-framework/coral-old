/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "RefCounted.h"
#include <cassert>
#include <iostream>

namespace co {

RefCounted::~RefCounted()
{
	if( _refCount != 0 )
	{
		// TODO call a handler function instead
		std::cerr << "Deleting object " << this
			<< " with a reference count of " << _refCount << ". Memory corruption possible.";

		assert( false );
	}
}

} // namespace co
