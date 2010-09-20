/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/MethodBuilder.h>
#include <co/DynamicProxyHandler.h>
#include <co/TypeBuilder.h>
#include <co/Type.h>
#include <co/ExceptionType.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class MethodBuilder_Proxy : public co::MethodBuilder
{
public:
	MethodBuilder_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::MethodBuilder>( this ) );
	}

	virtual ~MethodBuilder_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::MethodBuilder>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.MethodBuilder Methods:

	const std::string& getMethodName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodBuilder>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::TypeBuilder* getTypeBuilder()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodBuilder>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::TypeBuilder* >();
	}

	void createMethod()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 2 ), range, res );
	}

	void defineException( co::ExceptionType* __exceptionType )
	{
		co::Any res, args[1];
		args[0].set< co::ExceptionType* >( __exceptionType );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 3 ), range, res );
	}

	void defineParameter( const std::string& __name, co::Type* __type, bool __input, bool __output )
	{
		co::Any res, args[4];
		args[0].set< const std::string& >( __name );
		args[1].set< co::Type* >( __type );
		args[2].set< bool >( __input );
		args[3].set< bool >( __output );
		co::ArrayRange<co::Any const> range( args, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 4 ), range, res );
	}

	void defineReturnType( co::Type* __type )
	{
		co::Any res, args[1];
		args[0].set< co::Type* >( __type );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 5 ), range, res );
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

class MethodBuilder_Reflector : public co::ReflectorBase
{
public:
	MethodBuilder_Reflector()
	{
		// empty
	}

	virtual ~MethodBuilder_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::MethodBuilder>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::MethodBuilder);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::MethodBuilder>( new co::MethodBuilder_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::MethodBuilder* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getMethodName() ); break;
		case 1:		value.set< co::TypeBuilder* >( p->getTypeBuilder() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::MethodBuilder* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::MethodBuilder* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 2:
				{
					p->createMethod();
				}
				break;
			case 3:
				{
					co::ExceptionType* __exceptionType = args[++argIndex].get< co::ExceptionType* >();
					argIndex = -1;
					p->defineException( __exceptionType );
				}
				break;
			case 4:
				{
					const std::string& __name = args[++argIndex].get< const std::string& >();
					co::Type* __type = args[++argIndex].get< co::Type* >();
					bool __input = args[++argIndex].get< bool >();
					bool __output = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineParameter( __name, __type, __input, __output );
				}
				break;
			case 5:
				{
					co::Type* __type = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					p->defineReturnType( __type );
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
	co::MethodBuilder* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::MethodBuilder>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::MethodBuilder*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::MethodBuilder*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodBuilderReflector()
{
    return new MethodBuilder_Reflector;
}

} // namespace co
