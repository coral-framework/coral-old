/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_RESERVED_REFLECTORBASE_H_
#define _CO_RESERVED_REFLECTORBASE_H_

#include "ComponentBase.h"
#include <co/IField.h>
#include <co/IMethod.h>
#include <co/IInterface.h>
#include <co/IReflector.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

namespace co {

// forward declarations:
class IllegalCastException;

//! ReflectorBase provides a facet named 'reflector', of type co::IReflector.
class CORAL_EXPORT ReflectorBase_co_Reflector : public IReflector
{
public:
	virtual IInterface* getInterface();
	virtual IPort* getFacet();
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
    IObject* getProvider();
    void serviceRetain();
    void serviceRelease();

    // co::IObject methods:
    IComponent* getComponent();
    IService* getServiceAt( IPort* );
    void setServiceAt( IPort*, IService* );

	// co::IReflector methods:
	void createValues( void* ptr, size_t numValues );
    void copyValues( const void* fromPtr, void* toPtr, size_t numValues );
    void destroyValues( void* ptr, size_t numValues );
    IObject* newInstance();
    IService* newDynamicProxy( IDynamicServiceProvider* handler );
    void getField( const Any& instance, IField* ai, Any& value );
    void setField( const Any& instance, IField* ai, const Any& value );
    void invoke( const Any& instance, IMethod* mi, Range<Any const> args, Any& returnValue );
	void raise( const std::string& message );

protected:
	template<typename T>
	void callDestructor( T* address )
	{
		address->~T();
	}

	//! Raises co::IllegalArgumentException if handler is NULL.
	void checkValidDynamicProvider( IDynamicServiceProvider* provider );

	//! Raises co::MissingInputException if \a numArgs is lesser than \a mi's expected number of args.
	void checkNumArguments( IMethod* mi, size_t numArgs );

	//! Raises an exception because setField() was called on a read-only field.
	void raiseFieldIsReadOnly( IField* ai );

	//! Re-raises a co::IllegalCastException with info about which method parameter raised the exception.
	void raiseArgumentTypeException( IMethod* mi, int argIndex, const IllegalCastException& e );

	/*!
		Raises a co::IllegalArgumentException for cases (that "should never happen")
		where a member index is out of range.
	 */
	void raiseUnexpectedMemberIndex();

	//! Raises co::NotSupportedException, e.g. for unimplemented methods.
	void raiseNotSupportedException();
};

/****************************************************************************/
/* Auxiliary templates used internally by the generated reflectors          */
/****************************************************************************/

#ifndef DOXYGEN

namespace {
template<typename M> struct nullMemberMsg { static const char* get() { return "illegal null member"; } };
template<> struct nullMemberMsg<IField> { static const char* get() { return "illegal null field"; } };
template<> struct nullMemberMsg<IMethod> { static const char* get() { return "illegal null method"; } };
}

template<typename M>
void checkMember( ICompositeType* ct, M* member )
{
	if( !member )
		throw co::IllegalArgumentException( nullMemberMsg<M>::get() );

	ICompositeType* owner = member->getOwner();
	if( owner != ct )
		CORAL_THROW( IllegalArgumentException, "member '" << member->getName()
			<< "' belongs to '" << owner->getFullName()
			<< "', not to '" << ct->getFullName() << "'" );
}

template<typename T, typename TType>
struct CheckInstance
{
	static T* check( const Any& any, TType* type )
	{
		if( any.getKind() != kindOf<T>::kind || any.getType() != type )
			CORAL_THROW( IllegalArgumentException, "illegal instance type ("
				<< type->getFullName() << " expected, got " << any << ")" );

		void* ptr = any.getState().data.ptr;
		if( !ptr )
			throw co::IllegalArgumentException( "illegal null instance" );

		return reinterpret_cast<T*>( ptr );
	}
};

template<typename T>
struct CheckInstance<T, IInterface>
{
	static T* check( const Any& any, IInterface* type )
	{
		if( any.getKind() != TK_INTERFACE || !any.getInterface()->isSubTypeOf( type ) )
			CORAL_THROW( IllegalArgumentException, "illegal instance type ("
				<< type->getFullName() << " expected, got " << any << ")" );

		IService* service = any.getState().data.service;
		if( !service )
			throw co::IllegalArgumentException( "illegal null instance" );

		return static_cast<T*>( service );
	}
};

template<typename T, typename M>
T* checkInstance( const Any& any, M* member )
{
	typedef typeOf<T> typeOfT;
	typedef typename typeOfT::Descriptor Descriptor;
	Descriptor* type = typeOfT::get();
	checkMember( type, member );
	return CheckInstance<T, Descriptor>::check( any, type );
}

#endif

} // namespace co

#endif // _CO_RESERVED_REFLECTORBASE_H_
