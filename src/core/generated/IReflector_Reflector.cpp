/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IReflector.h>
#include <co/IField.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IType.h>
#include <co/IObject.h>
#include <co/IMethod.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/NotSupportedException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IReflector_Proxy : public co::IReflector
{
public:
	IReflector_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::IReflector>( this ) );
	}

	virtual ~IReflector_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::IReflector>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IReflector Methods:

	co::int32 getSize()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IReflector>( 0 ) );
        return res.get< co::int32 >();
	}

	co::IType* getType()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IReflector>( 1 ) );
        return res.get< co::IType* >();
	}

	void getAttribute( const co::Any& instance_, co::IField* ai_, co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IField* >( ai_ );
		args[2].set< co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 0 ), range );
	}

	void invokeMethod( const co::Any& instance_, co::IMethod* mi_, co::Range<co::Any const> args_, co::Any& returnValue_ )
	{
		co::Any args[4];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IMethod* >( mi_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		args[3].set< co::Any& >( returnValue_ );
		co::Range<co::Any const> range( args, 4 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 1 ), range );
	}

	co::IObject* newInstance()
	{
		co::Range<co::Any const> range;
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 2 ), range );
		return res.get< co::IObject* >();
	}

	co::IService* newProxy( co::IDynamicServiceProvider* handler_ )
	{
		co::Any args[1];
		args[0].set< co::IDynamicServiceProvider* >( handler_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 3 ), range );
		return res.get< co::IService* >();
	}

	void raise( const std::string& message_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( message_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 4 ), range );
	}

	void setAttribute( const co::Any& instance_, co::IField* ai_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< const co::Any& >( instance_ );
		args[1].set< co::IField* >( ai_ );
		args[2].set< const co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IReflector>( 5 ), range );
	}

	// These co::IReflector methods are not part of the reflection system:

	void createValue( void*, size_t )
	{
		throw co::NotSupportedException( "co::IReflector::createValue() cannot be called through a proxy interface." );
	}

	void copyValue( const void*, void* )
	{
		throw co::NotSupportedException( "co::IReflector::copyValue() cannot be called through a proxy interface." );
	}

	void destroyValue( void* )
	{
		throw co::NotSupportedException( "co::IReflector::destroyValue() cannot be called through a proxy interface." );
	}

protected:
	template<typename T>
	co::IField* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getFields()[index];
	}

	template<typename T>
	co::IMethod* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMethods()[index];
	}

private:
	co::IDynamicServiceProvider* _provider;
	co::uint32 _cookie;
};

// ------ Reflector Component ------ //

class IReflector_Reflector : public co::ReflectorBase
{
public:
	IReflector_Reflector()
	{
		// empty
	}

	virtual ~IReflector_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IReflector>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IReflector);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IReflector>( new co::IReflector_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::IReflector* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::int32 >( p->getSize() ); break;
		case 1:		value.set< co::IType* >( p->getType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::IReflector* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		co::IReflector* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 2:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IField* ai_ = args[++argIndex].get< co::IField* >();
					co::Any& value_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->getAttribute( instance_, ai_, value_ );
				}
				break;
			case 3:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IMethod* mi_ = args[++argIndex].get< co::IMethod* >();
					co::Range<co::Any const> args_ = args[++argIndex].get< co::Range<co::Any const> >();
					co::Any& returnValue_ = args[++argIndex].get< co::Any& >();
					argIndex = -1;
					p->invokeMethod( instance_, mi_, args_, returnValue_ );
				}
				break;
			case 4:
				{
					res.set< co::IObject* >( p->newInstance() );
				}
				break;
			case 5:
				{
					co::IDynamicServiceProvider* handler_ = args[++argIndex].get< co::IDynamicServiceProvider* >();
					argIndex = -1;
					res.set< co::IService* >( p->newProxy( handler_ ) );
				}
				break;
			case 6:
				{
					const std::string& message_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->raise( message_ );
				}
				break;
			case 7:
				{
					const co::Any& instance_ = args[++argIndex].get< const co::Any& >();
					co::IField* ai_ = args[++argIndex].get< co::IField* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->setAttribute( instance_, ai_, value_ );
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
	co::IReflector* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IReflector>::get();

		co::IReflector* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IReflector*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IReflector*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IReflector" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIReflectorReflector()
{
    return new IReflector_Reflector;
}

} // namespace co
