/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IANNOTATED_H_
#define _CO_IANNOTATED_H_

#include <co/Common.h>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IAnnotation; typedef co::RefPtr<IAnnotation> IAnnotationRef;
	class IInterface; typedef co::RefPtr<IInterface> IInterfaceRef;
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

	virtual co::TSlice<co::IAnnotation*> getAnnotations() = 0;

	virtual void setAnnotations( co::Slice<co::IAnnotation*> annotations ) = 0;

	virtual void addAnnotation( co::IAnnotation* annotation ) = 0;

	virtual co::IAnnotation* getAnnotation( co::IInterface* requestedType ) = 0;
};

typedef co::RefPtr<IAnnotated> IAnnotatedRef;

} // namespace co

namespace co {
template<> struct kindOf<co::IAnnotated> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IAnnotated> { static const char* get() { return "co.IAnnotated"; } };
} // namespace co

#endif // _CO_IANNOTATED_H_
