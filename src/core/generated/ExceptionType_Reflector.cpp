/*******************************************************************************
** Reflection code generated for type 'co.ExceptionType'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ExceptionType.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/Namespace.h>
#include <co/Reflector.h>
#include <co/Uuid.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Proxy Interface ------ //

class ExceptionType_Proxy : public co::ExceptionType
{
public:
	ExceptionType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ExceptionType>( this ) );
	}

	virtual ~ExceptionType_Proxy()
	{
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ExceptionType>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::Type Methods:

	const co::Uuid& getBinarySignature()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 0 ), __res );
        return __res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 1 ), __res );
        return __res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 2 ), __res );
        return __res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 3 ), __res );
        return __res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 4 ), __res );
        return __res.get< const std::string& >();
	}

	co::Namespace* getNamespace()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 5 ), __res );
        return __res.get< co::Namespace* >();
	}

	co::Reflector* getReflector()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), __res );
        return __res.get< co::Reflector* >();
	}

	void setReflector( co::Reflector* reflector )
	{
		co::Any __arg;
		__arg.set< co::Reflector* >( reflector );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), __arg );
	}

	// co::ExceptionType Methods:

protected:
	template<typename T>
	co::AttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::MethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::DynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ Reflector ------ //

class ExceptionType_Reflector : public co::ReflectorBase
{
public:
	ExceptionType_Reflector()
	{
		// empty
	}

	virtual ~ExceptionType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ExceptionType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ExceptionType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ExceptionType>( new ExceptionType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( __res );
	}

private:
	co::ExceptionType* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::ExceptionType>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ExceptionType*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::ExceptionType*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createExceptionTypeReflector()
{
    return new ExceptionType_Reflector;
}

} // namespace __coral
