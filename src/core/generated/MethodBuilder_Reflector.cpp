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
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodBuilder>( 0 ) );
        return res.get< const std::string& >();
	}

	co::TypeBuilder* getTypeBuilder()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodBuilder>( 1 ) );
        return res.get< co::TypeBuilder* >();
	}

	void createMethod()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 0 ), range );
	}

	void defineException( co::ExceptionType* exceptionType_ )
	{
		co::Any args[1];
		args[0].set< co::ExceptionType* >( exceptionType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 1 ), range );
	}

	void defineParameter( const std::string& name_, co::Type* type_, bool input_, bool output_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::Type* >( type_ );
		args[2].set< bool >( input_ );
		args[3].set< bool >( output_ );
		co::ArrayRange<co::Any const> range( args, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 2 ), range );
	}

	void defineReturnType( co::Type* type_ )
	{
		co::Any args[1];
		args[0].set< co::Type* >( type_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 3 ), range );
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
					co::ExceptionType* exceptionType_ = args[++argIndex].get< co::ExceptionType* >();
					argIndex = -1;
					p->defineException( exceptionType_ );
				}
				break;
			case 4:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::Type* type_ = args[++argIndex].get< co::Type* >();
					bool input_ = args[++argIndex].get< bool >();
					bool output_ = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineParameter( name_, type_, input_, output_ );
				}
				break;
			case 5:
				{
					co::Type* type_ = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					p->defineReturnType( type_ );
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
	co::MethodBuilder* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::MethodBuilder>::get();

		co::MethodBuilder* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::MethodBuilder*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::MethodBuilder*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.MethodBuilder" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodBuilderReflector()
{
    return new MethodBuilder_Reflector;
}

} // namespace co
