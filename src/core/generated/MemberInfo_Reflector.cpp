/*******************************************************************************
** Reflection code generated for type 'co.MemberInfo'
**
** Created: Thu Jun 10 20:53:30 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/MemberInfo.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/CompoundType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Proxy Interface ------ //

class MemberInfo_Proxy : public co::MemberInfo
{
public:
	MemberInfo_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::MemberInfo>( this ) );
	}

	virtual ~MemberInfo_Proxy()
	{
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::MemberInfo>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::MemberInfo Methods:

	co::uint32 getIndex()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 0 ), __res );
        return __res.get< co::uint32 >();
	}

	const std::string& getName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 1 ), __res );
        return __res.get< const std::string& >();
	}

	co::CompoundType* getOwner()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MemberInfo>( 2 ), __res );
        return __res.get< co::CompoundType* >();
	}

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

class MemberInfo_Reflector : public co::ReflectorBase
{
public:
	MemberInfo_Reflector()
	{
		// empty
	}

	virtual ~MemberInfo_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MemberInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MemberInfo);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::MemberInfo>( new MemberInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::MemberInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::uint32 >( p->getIndex() ); break;
		case 1:		value.set< const std::string& >( p->getName() ); break;
		case 2:		value.set< co::CompoundType* >( p->getOwner() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::MemberInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
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
	co::MemberInfo* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::MemberInfo>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::MemberInfo*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::MemberInfo*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createMemberInfoReflector()
{
    return new MemberInfo_Reflector;
}

} // namespace __coral
