/*******************************************************************************
** C++ mapping generated for type 'co.Reflector'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#ifndef _CO_REFLECTOR_H_
#define _CO_REFLECTOR_H_

#include <co/TypeTraits.h>
#include <co/Any.h>
#include <co/ArrayRange.h>
#include <co/Interface.h>

// Forward Declarations:
namespace co {
	class AttributeInfo;
	class Component;
	class DynamicProxyHandler;
	class MethodInfo;
	class Type;
} // namespace co
// End Of Forward Declarations

// co.Reflector Mapping:
namespace co {

class Reflector : public co::Interface
{
public:
	virtual ~Reflector() {;}

	// Code From <c++ Block:
	
		// Placement construction and destruction of complex Coral values is only available in C++.

		/*!
			Constructs an instance of a struct or native class in the specified memory area.
			\param[in] address memory address where the instance is to be constructed.
			\param[in] length number of bytes (starting at \c address) reserved for the instance.
			\throw UnsupportedOperationException if \c type is neither a co::StructType nor a co::NativeClassType.
			\throw IllegalArgumentException if the passed \c length is not equal to the type's \c size.
		 */
		virtual void createValue( void* address, size_t length ) = 0;

		/*!
			Destroys the struct or native class instance located at \c address.
			The instance should have been constructed using createValue().
			\warning Expect the worst if \c address does not point to an instance of this exact same type.
			\throw UnsupportedOperationException if \c type is neither a co::StructType nor a co::NativeClassType.
		 */
		virtual void destroyValue( void* address ) = 0;
	
	// End Of c++> Block

	virtual co::int32 getSize() = 0;

	virtual co::Type* getType() = 0;

	virtual void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value ) = 0;

	virtual void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& returnValue ) = 0;

	virtual co::Component* newInstance() = 0;

	virtual co::Interface* newProxy( co::DynamicProxyHandler* handler ) = 0;

	virtual void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value ) = 0;
};

} // namespace co

namespace co {
template<> struct kindOf<co::Reflector> : public kindOfBase<TK_INTERFACE> {};
template<> struct nameOf<co::Reflector> { static const char* get() { return "co.Reflector"; } };
template<> struct typeOf<co::Reflector> : public typeOfBase<co::Reflector, InterfaceType> {};
} // namespace co

#endif // _CO_REFLECTOR_H_
