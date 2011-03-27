/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IDynamicServiceProvider.h>
#include <co/IField.h>
#include <co/IMethod.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IDynamicServiceProvider_Proxy : public co::IDynamicServiceProvider
{
public:
	IDynamicServiceProvider_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::IDynamicServiceProvider>( this ) );
	}

	virtual ~IDynamicServiceProvider_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::IDynamicServiceProvider>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IDynamicServiceProvider Methods:

	const std::string& getProxyInterfaceName( co::int32 cookie_ )
	{
		co::Any args[1];
		args[0].set< co::int32 >( cookie_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IDynamicServiceProvider>( 0 ), range );
		return res.get< const std::string& >();
	}

	const co::Any& handleGetAttribute( co::int32 cookie_, co::IField* ai_ )
	{
		co::Any args[2];
		args[0].set< co::int32 >( cookie_ );
		args[1].set< co::IField* >( ai_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IDynamicServiceProvider>( 1 ), range );
		return res.get< const co::Any& >();
	}

	const co::Any& handleMethodInvocation( co::int32 cookie_, co::IMethod* mi_, co::Range<co::Any const> args_ )
	{
		co::Any args[3];
		args[0].set< co::int32 >( cookie_ );
		args[1].set< co::IMethod* >( mi_ );
		args[2].set< co::Range<co::Any const> >( args_ );
		co::Range<co::Any const> range( args, 3 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IDynamicServiceProvider>( 2 ), range );
		return res.get< const co::Any& >();
	}

	void handleSetAttribute( co::int32 cookie_, co::IField* ai_, const co::Any& value_ )
	{
		co::Any args[3];
		args[0].set< co::int32 >( cookie_ );
		args[1].set< co::IField* >( ai_ );
		args[2].set< const co::Any& >( value_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IDynamicServiceProvider>( 3 ), range );
	}

	co::int32 registerProxyInterface( co::IService* proxy_ )
	{
		co::Any args[1];
		args[0].set< co::IService* >( proxy_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::IDynamicServiceProvider>( 4 ), range );
		return res.get< co::int32 >();
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

class IDynamicServiceProvider_Reflector : public co::ReflectorBase
{
public:
	IDynamicServiceProvider_Reflector()
	{
		// empty
	}

	virtual ~IDynamicServiceProvider_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IDynamicServiceProvider>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IDynamicServiceProvider);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IDynamicServiceProvider>( new co::IDynamicServiceProvider_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		checkInstance( instance, ai );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		co::IDynamicServiceProvider* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::int32 cookie_ = args[++argIndex].get< co::int32 >();
					argIndex = -1;
					res.set< const std::string& >( p->getProxyInterfaceName( cookie_ ) );
				}
				break;
			case 1:
				{
					co::int32 cookie_ = args[++argIndex].get< co::int32 >();
					co::IField* ai_ = args[++argIndex].get< co::IField* >();
					argIndex = -1;
					res.set< const co::Any& >( p->handleGetAttribute( cookie_, ai_ ) );
				}
				break;
			case 2:
				{
					co::int32 cookie_ = args[++argIndex].get< co::int32 >();
					co::IMethod* mi_ = args[++argIndex].get< co::IMethod* >();
					co::Range<co::Any const> args_ = args[++argIndex].get< co::Range<co::Any const> >();
					argIndex = -1;
					res.set< const co::Any& >( p->handleMethodInvocation( cookie_, mi_, args_ ) );
				}
				break;
			case 3:
				{
					co::int32 cookie_ = args[++argIndex].get< co::int32 >();
					co::IField* ai_ = args[++argIndex].get< co::IField* >();
					const co::Any& value_ = args[++argIndex].get< const co::Any& >();
					argIndex = -1;
					p->handleSetAttribute( cookie_, ai_, value_ );
				}
				break;
			case 4:
				{
					co::IService* proxy_ = args[++argIndex].get< co::IService* >();
					argIndex = -1;
					res.set< co::int32 >( p->registerProxyInterface( proxy_ ) );
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
	co::IDynamicServiceProvider* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IDynamicServiceProvider>::get();

		co::IDynamicServiceProvider* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IDynamicServiceProvider*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IDynamicServiceProvider*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IDynamicServiceProvider" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIDynamicServiceProviderReflector()
{
    return new IDynamicServiceProvider_Reflector;
}

} // namespace co
