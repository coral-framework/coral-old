/*******************************************************************************
** Reflection code generated for type 'co.MethodBuilder'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/MethodBuilder.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/ExceptionType.h>
#include <co/Type.h>
#include <co/TypeBuilder.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
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

	// co::MethodBuilder Methods:

	const std::string& getMethodName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodBuilder>( 0 ), __res );
        return __res.get< const std::string& >();
	}

	co::TypeBuilder* getTypeBuilder()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::MethodBuilder>( 1 ), __res );
        return __res.get< co::TypeBuilder* >();
	}

	void createMethod( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 2 ), __ar, __res );
	}

	void defineException( co::ExceptionType* exceptionType )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::ExceptionType* >( exceptionType );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 3 ), __ar, __res );
	}

	void defineParameter( const std::string& name, co::Type* type, bool input, bool output )
	{
		co::Any __res, __arg[4];
		__arg[0].set< const std::string& >( name );
		__arg[1].set< co::Type* >( type );
		__arg[2].set< bool >( input );
		__arg[3].set< bool >( output );
		co::ArrayRange<co::Any const> __ar( __arg, 4 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 4 ), __ar, __res );
	}

	void defineReturnType( co::Type* type )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::Type* >( type );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::MethodBuilder>( 5 ), __ar, __res );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
					co::ExceptionType* exceptionType = args[++argIndex].get< co::ExceptionType* >();
					argIndex = -1;
					p->defineException( exceptionType );
				}
				break;
			case 4:
				{
					const std::string& name = args[++argIndex].get< const std::string& >();
					co::Type* type = args[++argIndex].get< co::Type* >();
					bool input = args[++argIndex].get< bool >();
					bool output = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineParameter( name, type, input, output );
				}
				break;
			case 5:
				{
					co::Type* type = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					p->defineReturnType( type );
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
		CORAL_UNUSED( __res );
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

		return reinterpret_cast<co::MethodBuilder*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createMethodBuilderReflector()
{
    return new MethodBuilder_Reflector;
}

} // namespace co
