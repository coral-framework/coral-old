/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_IREFLECTOR_H_
#define _CO_IREFLECTOR_H_

#include <co/TypeTraits.h>
#include <co/ArrayRange.h>
#include <co/Any.h>
#include <vector>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class IAttributeInfo;
	class IComponent;
	class IDynamicProxyHandler;
	class IMethodInfo;
	class IType;
} // namespace co
// End Of Forward Declarations

// co.IReflector Mapping:
namespace co {

class IReflector : public co::Interface
{
public:
	virtual ~IReflector() {;}

	// Code From <c++ Block:
	
		// Memory manipulation of complex Coral values is only available from C++.

		/*!
			Constructs an instance of a struct or native class in the specified memory area.
			\param[in] address memory address where the instance is to be constructed.
			\param[in] length number of bytes (starting at \a address) reserved for the instance.
			\throw NotSupportedException if \a type is neither a IStructType nor a INativeClassType.
			\throw IllegalArgumentException if the passed \a length is not equal to the type's \a size.
		 */
		virtual void createValue( void* address, size_t length ) = 0;

		/*!
			Sets the complex value at 'toAddress' with a copy of the complex value at 'fromAddress'.
			The copy is done using the type's assignment operator.
			\warning Both pointers must point to instances of this exact 'type'.
		 */
		virtual void copyValue( const void* fromAddress, void* toAddress ) = 0;

		/*!
			Destroys the struct or native class instance located at \a address.
			The instance should have been constructed using createValue().
			\warning Expect the worst if \a address does not point to an instance of this exact type.
			\throw NotSupportedException if \a type is neither a IStructType nor a INativeClassType.
		 */
		virtual void destroyValue( void* address ) = 0;
	
	// End Of c++> Block

	virtual co::int32 getSize() = 0;

	virtual co::IType* getType() = 0;

	virtual void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value ) = 0;

	virtual void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue ) = 0;

	virtual co::IComponent* newInstance() = 0;

	virtual co::Interface* newProxy( co::IDynamicProxyHandler* handler ) = 0;

	virtual void raise( const std::string& message ) = 0;

	virtual void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::IReflector> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::IReflector> { static const char* get() { return "co.IReflector"; } };
template<> struct typeOf<co::IReflector> : public typeOfBase<co::IReflector, IInterfaceType> {};
} // namespace co

#endif // _CO_IREFLECTOR_H_
