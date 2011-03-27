/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ISystem.h>
#include <co/IDynamicServiceProvider.h>
#include <co/ITypeManager.h>
#include <co/IServiceManager.h>
#include <co/IModuleManager.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class ISystem_Proxy : public co::ISystem
{
public:
	ISystem_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::ISystem>( this ) );
	}

	virtual ~ISystem_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::ISystem>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.ISystem Methods:

	co::IModuleManager* getModules()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ISystem>( 0 ) );
        return res.get< co::IModuleManager* >();
	}

	co::IServiceManager* getServices()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ISystem>( 1 ) );
        return res.get< co::IServiceManager* >();
	}

	co::SystemState getState()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ISystem>( 2 ) );
        return res.get< co::SystemState >();
	}

	co::ITypeManager* getTypes()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ISystem>( 3 ) );
        return res.get< co::ITypeManager* >();
	}

	void setupBase( co::Range<std::string const> requiredModules_ )
	{
		co::Any args[1];
		args[0].set< co::Range<std::string const> >( requiredModules_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::ISystem>( 0 ), range );
	}

	void setupPresentation()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::ISystem>( 1 ), range );
	}

	void tearDown()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::ISystem>( 2 ), range );
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

class ISystem_Reflector : public co::ReflectorBase
{
public:
	ISystem_Reflector()
	{
		// empty
	}

	virtual ~ISystem_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ISystem>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ISystem);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::ISystem>( new co::ISystem_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::ISystem* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::IModuleManager* >( p->getModules() ); break;
		case 1:		value.set< co::IServiceManager* >( p->getServices() ); break;
		case 2:		value.set< co::SystemState >( p->getState() ); break;
		case 3:		value.set< co::ITypeManager* >( p->getTypes() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::ISystem* p = checkInstance( instance, ai );
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

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		co::ISystem* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 4:
				{
					co::Range<std::string const> requiredModules_ = args[++argIndex].get< co::Range<std::string const> >();
					argIndex = -1;
					p->setupBase( requiredModules_ );
				}
				break;
			case 5:
				{
					p->setupPresentation();
				}
				break;
			case 6:
				{
					p->tearDown();
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
	co::ISystem* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::ISystem>::get();

		co::ISystem* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ISystem*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ISystem*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ISystem" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createISystemReflector()
{
    return new ISystem_Reflector;
}

} // namespace co
