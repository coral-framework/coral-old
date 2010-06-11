/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include "RefCounted.h"
#include <glog/logging.h>
#include <cassert>

namespace co {

RefCounted::~RefCounted()
{
	if( _refCount != 0 )
	{
		LOG( ERROR ) << "Deleting object " << this
			<< " with a reference count of " << _refCount << ". Memory corruption possible.";
		
		assert( false );
	}
}

} // namespace co
