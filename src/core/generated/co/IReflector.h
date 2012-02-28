/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IREFLECTOR_H_
#define _CO_IREFLECTOR_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/Range.h>
#include <vector>
#include <co/IService.h>

// Forward Declarations:
namespace co {
	class IDynamicServiceProvider;
	class IField;
	class IMethod;
	class IObject;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IReflector Mapping:
namespace co {

class IReflector : public co::IService
{
public:
	virtual ~IReflector() {;}

	// Code From <c++ Block:
	
		//------ Manipulation of Values (only available from C++) ------//

		/*!
			Constructs an array of values in the memory area starting at \a ptr.
			\pre The memory area at \a ptr should be at least \a numValues * \a size bytes big.
			\throw NotSupportedException if this \a type does not have a value form.
		 */
		virtual void createValues( void* ptr, size_t numValues ) = 0;

		/*!
			Copies an array of \a numValues values from \a fromPtr to \a toPtr.
			\note Each copy is done using the value's assignment operator.
			\pre Both arrays must contain at least \a numValues properly constructed values of this type.
			\throw NotSupportedException if this \a type does not have a value form.
		 */
		virtual void copyValues( const void* fromPtr, void* toPtr, size_t numValues ) = 0;

		/*!
			Destructs an array of \a numValues values starting at \a ptr.
			\pre The array must contain at least \a numValues values constructed with createValues().
			\throw NotSupportedException if this \a type does not have a value form.
		 */
		virtual void destroyValues( void* ptr, size_t numValues ) = 0;
	
	// End Of c++> Block

	virtual co::uint32 getSize() = 0;

	virtual co::IType* getType() = 0;

	virtual void getField( co::Any instance, co::IField* field, co::AnyValue& value ) = 0;

	virtual void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any const> args, co::AnyValue& returnValue ) = 0;

	virtual co::IService* newDynamicProxy( co::IDynamicServiceProvider* dynamicProvider ) = 0;

	virtual co::IObject* newInstance() = 0;

	virtual void raise( const std::string& message ) = 0;

	virtual void setField( co::Any instance, co::IField* field, co::Any value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IReflector> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IReflector> { static const char* get() { return "co.IReflector"; } };
} // namespace co

#endif // _CO_IREFLECTOR_H_
