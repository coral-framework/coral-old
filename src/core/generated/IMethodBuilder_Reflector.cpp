/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMethodBuilder.h>
#include <co/IDynamicProxyHandler.h>
#include <co/ITypeBuilder.h>
#include <co/IExceptionType.h>
#include <co/IType.h>
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class IMethodBuilder_Proxy : public co::IMethodBuilder
{
public:
	IMethodBuilder_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IMethodBuilder>( this ) );
	}

	virtual ~IMethodBuilder_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IMethodBuilder>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IMethodBuilder Methods:

	const std::string& getMethodName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IMethodBuilder>( 0 ) );
        return res.get< const std::string& >();
	}

	co::ITypeBuilder* getTypeBuilder()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IMethodBuilder>( 1 ) );
        return res.get< co::ITypeBuilder* >();
	}

	void createMethod()
	{
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IMethodBuilder>( 0 ), range );
	}

	void defineException( co::IExceptionType* exceptionType_ )
	{
		co::Any args[1];
		args[0].set< co::IExceptionType* >( exceptionType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IMethodBuilder>( 1 ), range );
	}

	void defineParameter( const std::string& name_, co::IType* type_, bool input_, bool output_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::IType* >( type_ );
		args[2].set< bool >( input_ );
		args[3].set< bool >( output_ );
		co::ArrayRange<co::Any const> range( args, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IMethodBuilder>( 2 ), range );
	}

	void defineReturnType( co::IType* type_ )
	{
		co::Any args[1];
		args[0].set< co::IType* >( type_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IMethodBuilder>( 3 ), range );
	}

protected:
	template<typename T>
	co::IAttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::IMethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::IDynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ IReflector ------ //

class IMethodBuilder_Reflector : public co::ReflectorBase
{
public:
	IMethodBuilder_Reflector()
	{
		// empty
	}

	virtual ~IMethodBuilder_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IMethodBuilder>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IMethodBuilder);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IMethodBuilder>( new co::IMethodBuilder_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IMethodBuilder* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getMethodName() ); break;
		case 1:		value.set< co::ITypeBuilder* >( p->getTypeBuilder() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IMethodBuilder* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IMethodBuilder* p = checkInstance( instance, mi );
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
					co::IExceptionType* exceptionType_ = args[++argIndex].get< co::IExceptionType* >();
					argIndex = -1;
					p->defineException( exceptionType_ );
				}
				break;
			case 4:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::IType* type_ = args[++argIndex].get< co::IType* >();
					bool input_ = args[++argIndex].get< bool >();
					bool output_ = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineParameter( name_, type_, input_, output_ );
				}
				break;
			case 5:
				{
					co::IType* type_ = args[++argIndex].get< co::IType* >();
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
	co::IMethodBuilder* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IMethodBuilder>::get();

		co::IMethodBuilder* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IMethodBuilder*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IMethodBuilder*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IMethodBuilder" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIMethodBuilderIReflector()
{
    return new IMethodBuilder_Reflector;
}

} // namespace co
