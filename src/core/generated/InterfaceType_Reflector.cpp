/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/InterfaceType.h>
#include <co/DynamicProxyHandler.h>
#include <co/InterfaceType.h>
#include <co/Namespace.h>
#include <co/AttributeInfo.h>
#include <co/MethodInfo.h>
#include <co/Reflector.h>
#include <co/MemberInfo.h>
#include <co/Uuid.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class InterfaceType_Proxy : public co::InterfaceType
{
public:
	InterfaceType_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::InterfaceType>( this ) );
	}

	virtual ~InterfaceType_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::InterfaceType>::get(); }
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

	co::MemberInfo* getMember( const std::string& __name )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __name );
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

	void setReflector( co::Reflector* __reflector )
	{
		co::Any arg;
		arg.set< co::Reflector* >( __reflector );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::Type>( 6 ), arg );
	}

	// co.MethodContainer Methods:

	co::ArrayRange<co::MethodInfo* const> getMemberMethods()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodContainer>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::MethodInfo* const> >();
	}

	// co.AttributeContainer Methods:

	co::ArrayRange<co::AttributeInfo* const> getMemberAttributes()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::AttributeContainer>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::AttributeInfo* const> >();
	}

	// co.InterfaceType Methods:

	const std::string& getCppBlock()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::ArrayRange<co::InterfaceType* const> getInterfaceAncestors()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	co::ArrayRange<co::InterfaceType* const> getSubInterfaces()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	co::ArrayRange<co::InterfaceType* const> getSuperInterfaces()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::InterfaceType>( 3 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::InterfaceType* const> >();
	}

	bool isSubTypeOf( co::InterfaceType* __itf )
	{
		co::Any res, args[1];
		args[0].set< co::InterfaceType* >( __itf );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::InterfaceType>( 4 ), range, res );
		assert( res.containsObject() == false );
		return res.get< bool >();
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

class InterfaceType_Reflector : public co::ReflectorBase
{
public:
	InterfaceType_Reflector()
	{
		// empty
	}

	virtual ~InterfaceType_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::InterfaceType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::InterfaceType);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::InterfaceType>( new co::InterfaceType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::InterfaceType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getCppBlock() ); break;
		case 1:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getInterfaceAncestors() ); break;
		case 2:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getSubInterfaces() ); break;
		case 3:		value.set< co::ArrayRange<co::InterfaceType* const> >( p->getSuperInterfaces() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::InterfaceType* p = checkInstance( instance, ai );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::InterfaceType* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 4:
				{
					co::InterfaceType* __itf = args[++argIndex].get< co::InterfaceType* >();
					argIndex = -1;
					res.set< bool >( p->isSubTypeOf( __itf ) );
				}
				break;
			default:
				raiseUnexpectedMemberIndex();
			}
		}
		catch( co::IllegalCastException& e )
		{
			if( argIndex == -1 )
				throw; // just re-throw if the exception is not related to 'args'
			raiseArgumentTypeException( mi, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
	}

private:
	co::InterfaceType* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::InterfaceType>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::InterfaceType*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::InterfaceType*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createInterfaceTypeReflector()
{
    return new InterfaceType_Reflector;
}

} // namespace co
