/*
 * Coral - Lightweight C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_REFLECTORBASE_H_
#define _CO_RESERVED_REFLECTORBASE_H_

#include "ComponentBase.h"
#include <co/Reflector.h>

namespace co {

// forward declarations:
class IllegalCastException;

//! ReflectorBase provides an interface named 'reflector', of type co::Reflector.
class CORAL_EXPORT ReflectorBase_co_Reflector : public Reflector
{
public:
	virtual InterfaceType* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement a Reflector.
 */
class CORAL_EXPORT ReflectorBase : public ComponentBase, public ReflectorBase_co_Reflector
{
public:
	ReflectorBase();
	virtual ~ReflectorBase();

	// co::Interface methods:
    Component* getInterfaceOwner();
    void componentRetain();
    void componentRelease();

    // co::Component methods:
    ComponentType* getComponentType();
    Interface* getInterface( InterfaceInfo* );
    void bindInterface( InterfaceInfo*, Interface* );

	// co::Reflector methods:
	void createValue( void* address, size_t length );
    void copyValue( const void* fromAddress, void* toAddress );
    void destroyValue( void* address );
    Component* newInstance();
    Interface* newProxy( DynamicProxyHandler* handler );
    void getAttribute( const Any& instance, AttributeInfo* ai, Any& value );
    void setAttribute( const Any& instance, AttributeInfo* ai, const Any& value );
    void invokeMethod( const Any& instance, MethodInfo* mi, ArrayRange<Any const> args, Any& returnValue );

protected:
	template<typename T>
	void callDestructor( T* address )
	{
		address->~T();
	}

	//! Useful in structs, for setting std::vector's from co::Any's.
	template<typename T, typename VT>
	void setArray( std::vector<VT>& vec, const co::Any& any )
	{
		vec.clear();
		co::ArrayRange<const T> ar( any.get<co::ArrayRange<const T> >() );
		vec.reserve( ar.getSize() );
		for( ; ar; ar.popFirst() )
			vec.push_back( ar.getFirst() );
	}

	//! Raises co::IllegalArgumentException if the passed sizes do not match.
	void checkValidSize( size_t expectedSize, size_t actualSize );

	//! Raises co::IllegalArgumentException if handler is NULL.
	void checValidProxyHandler( co::DynamicProxyHandler* handler );

	//! Raises co::MissingInputException if \a numArgs is lesser than \a mi's expected number of args.
	void checkNumArguments( co::MethodInfo* mi, std::size_t numArgs );

	//! Raises an exception because setAttribute() was called on a read-only attribute.
	void raiseAttributeIsReadOnly( co::AttributeInfo* ai );

	//! Re-raises a co::IllegalCastException with info about which method parameter raised the exception.
	void raiseArgumentTypeException( co::MethodInfo* mi, int argIndex, const co::IllegalCastException& e );

	/*!
		Raises a co::IllegalArgumentException for cases (that "should never happen")
		where a member index is out of range.
	 */
	void raiseUnexpectedMemberIndex();

	//! Raises co::UnsupportedOperationException, e.g. for unimplemented methods.
	void raiseUnsupportedOperationException();
};

} // namespace co

#endif // _CO_RESERVED_REFLECTORBASE_H_
