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
	
		//------ Manipulation of complex value instances (only available from C++) ------//

		/*!
			Constructs an instance of a struct or native class in the specified memory area.
			\param[in] address memory address where the instance is to be constructed.
			\throw NotSupportedException if the \a type is neither a struct nor a native class.
		 */
		virtual void createValue( void* address ) = 0;

		/*!
			Sets the complex value at \a toAddress with a copy of the complex value at \a fromAddress.
			The copy is done using the type's assignment operator.
			\warning Both pointers must point to instances of this exact type.
		 */
		virtual void copyValue( const void* fromAddress, void* toAddress ) = 0;

		/*!
			Destroys the struct or native class instance located at \a address.
			The instance should have been constructed using createValue().
			\warning Expect the worst if \a address does not point to an instance of this type.
			\throw NotSupportedException if the \a type is neither a struct nor a native class.
		 */
		virtual void destroyValue( void* address ) = 0;
	
	// End Of c++> Block

	virtual co::uint32 getSize() = 0;

	virtual co::IType* getType() = 0;

	virtual void getField( const co::Any& instance, co::IField* field, co::Any& value ) = 0;

	virtual void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& returnValue ) = 0;

	virtual co::IService* newDynamicProxy( co::IDynamicServiceProvider* dynamicProvider ) = 0;

	virtual co::IObject* newInstance() = 0;

	virtual void raise( const std::string& message ) = 0;

	virtual void setField( const co::Any& instance, co::IField* field, const co::Any& value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IReflector> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IReflector> { static const char* get() { return "co.IReflector"; } };
} // namespace co

#endif // _CO_IREFLECTOR_H_
