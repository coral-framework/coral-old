/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/StructType.h>
#include <co/DynamicProxyHandler.h>
#include <co/Reflector.h>
#include <co/Namespace.h>
#include <co/AttributeInfo.h>
#include <co/MemberInfo.h>
#include <co/Uuid.h>
#include <co/MethodInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class StructType_Proxy : public co::StructType
{
public:
	StructType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::StructType>( this ) );
	}

	virtual ~StructType_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::StructType>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.CompoundType Methods:

	co::ArrayRange<co::MemberInfo* const> getMembers()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::CompoundType>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::MemberInfo* const> >();
	}

	co::MemberInfo* getMember( const std::string& name_ )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( name_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::CompoundType>( 1 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::MemberInfo* >();
	}

	// co.Type Methods:

	const co::Uuid& getBinarySignature()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 3 ), res );
		assert( res.containsObject() == false );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 4 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::Namespace* getNamespace()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 5 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Namespace* >();
	}

	co::Reflector* getReflector()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Reflector* >();
	}

	void setReflector( co::Reflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::Reflector* >( reflector_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), arg );
	}

	// co.AttributeContainer Methods:

	co::ArrayRange<co::AttributeInfo* const> getMemberAttributes()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::AttributeContainer>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::AttributeInfo* const> >();
	}

	// co.StructType Methods:

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

class StructType_Reflector : public co::ReflectorBase
{
public:
	StructType_Reflector()
	{
		// empty
	}

	virtual ~StructType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::StructType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::StructType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::StructType>( new co::StructType_Proxy( handler ) );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::StructType* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::StructType>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::StructType*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::StructType*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createStructTypeReflector()
{
    return new StructType_Reflector;
}

} // namespace co
