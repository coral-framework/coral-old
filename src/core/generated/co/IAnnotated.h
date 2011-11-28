/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IANNOTATED_H_
#define _CO_IANNOTATED_H_

#include <co/TypeTraits.h>
#include <co/Range.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IAnnotation;
	class IInterface;
} // namespace co
// End Of Forward Declarations

// co.IAnnotated Mapping:
namespace co {

class IAnnotated : public co::IService
{
public:
	virtual ~IAnnotated() {;}

	// Code From <c++ Block:
	
		/*!
			Returns this service's annotation for the specified type,
			if such an annotation is present; else null.
			\tparam T any interface that extends co.IAnnotation.
		 */
		template<typename T> inline T* getAnnotation()
		{
			return static_cast<T*>( getAnnotation( co::typeOf<T>::get() ) );
		}
	
	// End Of c++> Block

	virtual co::Range<co::IAnnotation* const> getAnnotations() = 0;

	virtual void setAnnotations( co::Range<co::IAnnotation* const> annotations ) = 0;

	virtual void addAnnotation( co::IAnnotation* annotation ) = 0;

	virtual co::IAnnotation* getAnnotation( co::IInterface* requestedType ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IAnnotated> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IAnnotated> { static const char* get() { return "co.IAnnotated"; } };
} // namespace co

#endif // _CO_IANNOTATED_H_
