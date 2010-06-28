/*******************************************************************************
** Reflection code generated for type 'co.ComponentType'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/ComponentType.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/InterfaceInfo.h>
#include <co/MemberInfo.h>
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

class ComponentType_Proxy : public co::ComponentType
{
public:
	ComponentType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::ComponentType>( this ) );
	}

	virtual ~ComponentType_Proxy()
	{
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::ComponentType>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::CompoundType Methods:

	co::ArrayRange<co::MemberInfo* const> getMembers()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::CompoundType>( 0 ), __res );
        return __res.get< co::ArrayRange<co::MemberInfo* const> >();
	}

	co::MemberInfo* getMember( const std::string& name )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( name );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::CompoundType>( 1 ), __ar, __res );
		return __res.get< co::MemberInfo* >();
	}

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

	// co::ComponentType Methods:

	co::ArrayRange<co::InterfaceInfo* const> getInterfaces()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ComponentType>( 0 ), __res );
        return __res.get< co::ArrayRange<co::InterfaceInfo* const> >();
	}

	co::ArrayRange<co::InterfaceInfo* const> getProvidedInterfaces()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ComponentType>( 1 ), __res );
        return __res.get< co::ArrayRange<co::InterfaceInfo* const> >();
	}

	co::ArrayRange<co::InterfaceInfo* const> getRequiredInterfaces()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::ComponentType>( 2 ), __res );
        return __res.get< co::ArrayRange<co::InterfaceInfo* const> >();
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

class ComponentType_Reflector : public co::ReflectorBase
{
public:
	ComponentType_Reflector()
	{
		// empty
	}

	virtual ~ComponentType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::ComponentType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ComponentType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::ComponentType>( new ComponentType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::ComponentType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::InterfaceInfo* const> >( p->getInterfaces() ); break;
		case 1:		value.set< co::ArrayRange<co::InterfaceInfo* const> >( p->getProvidedInterfaces() ); break;
		case 2:		value.set< co::ArrayRange<co::InterfaceInfo* const> >( p->getRequiredInterfaces() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::ComponentType* p = checkInstance( instance, ai );
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
	co::ComponentType* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::ComponentType>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ComponentType*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::ComponentType*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createComponentTypeReflector()
{
    return new ComponentType_Reflector;
}

} // namespace __coral
