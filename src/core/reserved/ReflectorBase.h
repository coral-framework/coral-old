/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_REFLECTORBASE_H_
#define _CO_RESERVED_REFLECTORBASE_H_

#include "ComponentBase.h"
#include <co/IReflector.h>

namespace co {

// forward declarations:
class IllegalCastException;

//! ReflectorBase provides an interface named 'reflector', of type co::IReflector.
class CORAL_EXPORT ReflectorBase_co_Reflector : public IReflector
{
public:
	virtual IInterface* getInterfaceType();
	virtual const std::string& getInterfaceName();
};

/*!
	Inherit from this class to implement a IReflector.
 */
class CORAL_EXPORT ReflectorBase : public ComponentBase, public ReflectorBase_co_Reflector
{
public:
	ReflectorBase();
	virtual ~ReflectorBase();

	// co::IService methods:
    IObject* getInterfaceOwner();
    void componentRetain();
    void componentRelease();

    // co::IObject methods:
    IComponent* getComponentType();
    IService* getInterface( IPort* );
    void setReceptacle( IPort*, IService* );

	// co::IReflector methods:
	void createValue( void* address, size_t length );
    void copyValue( const void* fromAddress, void* toAddress );
    void destroyValue( void* address );
    IObject* newInstance();
    IService* newProxy( IDynamicServiceProvider* handler );
    void getAttribute( const Any& instance, IField* ai, Any& value );
    void setAttribute( const Any& instance, IField* ai, const Any& value );
    void invokeMethod( const Any& instance, IMethod* mi, Range<Any const> args, Any& returnValue );
	void raise( const std::string& message );

protected:
	template<typename T>
	void callDestructor( T* address )
	{
		address->~T();
	}

	//! Raises co::IllegalArgumentException if the passed sizes do not match.
	void checkValidSize( size_t expectedSize, size_t actualSize );

	//! Raises co::IllegalArgumentException if handler is NULL.
	void checkValidDynamicProvider( co::IDynamicServiceProvider* provider );

	//! Raises co::MissingInputException if \a numArgs is lesser than \a mi's expected number of args.
	void checkNumArguments( co::IMethod* mi, size_t numArgs );

	//! Raises an exception because setAttribute() was called on a read-only attribute.
	void raiseAttributeIsReadOnly( co::IField* ai );

	//! Re-raises a co::IllegalCastException with info about which method parameter raised the exception.
	void raiseArgumentTypeException( co::IMethod* mi, int argIndex, const co::IllegalCastException& e );

	/*!
		Raises a co::IllegalArgumentException for cases (that "should never happen")
		where a member index is out of range.
	 */
	void raiseUnexpectedMemberIndex();

	//! Raises co::NotSupportedException, e.g. for unimplemented methods.
	void raiseNotSupportedException();
};

} // namespace co

#endif // _CO_RESERVED_REFLECTORBASE_H_
