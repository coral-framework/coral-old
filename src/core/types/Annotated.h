/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ANNOTATED_H_
#define _CO_ANNOTATED_H_

#include <co/RefVector.h>
#include <co/IAnnotation.h>
#include <co/IllegalArgumentException.h>

namespace co {

/*!
	Re-usable implementation of co::IAnnotated.
 */
template<class Base>
class Annotated : public Base
{
public:
	inline void addAnnotation( co::IAnnotation* annotation )
	{
		_annotations.push_back( annotation );
	}

	co::IAnnotation* getAnnotation( co::IInterface* annotationType )
	{
		if( !annotationType )
			throw co::IllegalArgumentException( "illegal null annotationType" );

		size_t count = _annotations.size();
		for( size_t i = 0; i < count; ++i )
			if( _annotations[i]->getInterface() == annotationType )
				return _annotations[i].get();

		return NULL;
	}

	co::Range<co::IAnnotation* const> getAnnotations()
	{
		return _annotations;
	}

private:
	co::RefVector<co::IAnnotation> _annotations;
};

} // namespace co

#endif
