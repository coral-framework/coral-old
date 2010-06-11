/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_REFCOUNTED_H_
#define _CO_REFCOUNTED_H_

#include <co/Platform.h>
#include <co/RefPtr.h>

#include <ostream>

namespace co {

/*!
	General-purpose classes that need reference counting should inherit from this class.
	Should be used in conjunction with co::RefPtr.
 */
class CORAL_EXPORT RefCounted
{
public:
	RefCounted() : _refCount( 0 )
	{;}

	RefCounted( const RefCounted& ) : _refCount( 0 )
	{;}

	inline RefCounted& operator=( const RefCounted& )
	{
		return *this;
	}

	inline void componentRetain() const
	{
		++_refCount;
	}

	inline void componentRelease() const
	{
		if( --_refCount <= 0 )
			delete this;
	}

	inline int32 getRefCount() const
	{
		return _refCount;
	}

protected:
	virtual ~RefCounted();

private:
	mutable int32 _refCount;
};

} // namespace co

#endif
