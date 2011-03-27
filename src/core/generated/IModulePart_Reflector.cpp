/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModulePart.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IModule.h>
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

class IModulePart_Proxy : public co::IModulePart
{
public:
	IModulePart_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::IModulePart>( this ) );
	}

	virtual ~IModulePart_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::IModulePart>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IModulePart Methods:

	void disintegrate( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 0 ), range );
	}

	void dispose( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 1 ), range );
	}

	void initialize( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 2 ), range );
	}

	void integrate( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 3 ), range );
	}

	void integratePresentation( co::IModule* module_ )
	{
		co::Any args[1];
		args[0].set< co::IModule* >( module_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModulePart>( 4 ), range );
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

class IModulePart_Reflector : public co::ReflectorBase
{
public:
	IModulePart_Reflector()
	{
		// empty
	}

	virtual ~IModulePart_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModulePart>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IModulePart);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IModulePart>( new co::IModulePart_Proxy( provider ) );
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
		co::IModulePart* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 0:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->disintegrate( module_ );
				}
				break;
			case 1:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->dispose( module_ );
				}
				break;
			case 2:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->initialize( module_ );
				}
				break;
			case 3:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->integrate( module_ );
				}
				break;
			case 4:
				{
					co::IModule* module_ = args[++argIndex].get< co::IModule* >();
					argIndex = -1;
					p->integratePresentation( module_ );
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
	co::IModulePart* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IModulePart>::get();

		co::IModulePart* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IModulePart*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IModulePart*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IModulePart" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIModulePartReflector()
{
    return new IModulePart_Reflector;
}

} // namespace co
