/*******************************************************************************
** Reflection code generated for type 'co.ParameterInfo'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ParameterInfo.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/Type.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

// ------ Proxy Interface ------ //

class ParameterInfo_Proxy : public co::ParameterInfo
{
public:
	ParameterInfo_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ParameterInfo>( this ) );
	}

	virtual ~ParameterInfo_Proxy()
	{
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ParameterInfo>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::ParameterInfo Methods:

	bool getIsIn()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 0 ), __res );
        return __res.get< bool >();
	}

	bool getIsOut()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 1 ), __res );
        return __res.get< bool >();
	}

	const std::string& getName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 2 ), __res );
        return __res.get< const std::string& >();
	}

	co::Type* getType()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ParameterInfo>( 3 ), __res );
        return __res.get< co::Type* >();
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

class ParameterInfo_Reflector : public co::ReflectorBase
{
public:
	ParameterInfo_Reflector()
	{
		// empty
	}

	virtual ~ParameterInfo_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ParameterInfo>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ParameterInfo);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ParameterInfo>( new ParameterInfo_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::ParameterInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getIsIn() ); break;
		case 1:		value.set< bool >( p->getIsOut() ); break;
		case 2:		value.set< const std::string& >( p->getName() ); break;
		case 3:		value.set< co::Type* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ParameterInfo* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
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
	co::ParameterInfo* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::ParameterInfo>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ParameterInfo*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::ParameterInfo*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createParameterInfoReflector()
{
    return new ParameterInfo_Reflector;
}

} // namespace __coral
