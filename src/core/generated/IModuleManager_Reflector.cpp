/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModuleManager.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IModule.h>
#include <co/IModulePartLoader.h>
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

class IModuleManager_Proxy : public co::IModuleManager
{
public:
	IModuleManager_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IModuleManager_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IModuleManager>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IModuleManager Methods:

	bool getBinaryCompatibilityChecking()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IModuleManager>( 0 ) );
        return res.get< bool >();
	}

	void setBinaryCompatibilityChecking( bool binaryCompatibilityChecking_ )
	{
		co::Any arg;
		arg.set< bool >( binaryCompatibilityChecking_ );
		_provider->dynamicSetField( _cookie, getField<co::IModuleManager>( 0 ), arg );
	}

	co::Range<co::IModulePartLoader*> getLoaders()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IModuleManager>( 1 ) );
        return res.get< co::Range<co::IModulePartLoader*> >();
	}

	co::Range<co::IModule*> getModules()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IModuleManager>( 2 ) );
        return res.get< co::Range<co::IModule*> >();
	}

	co::IModule* findModule( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::Range<co::Any> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 0 ), range );
		return res.get< co::IModule* >();
	}

	void installLoader( co::IModulePartLoader* loader_ )
	{
		co::Any args[1];
		args[0].set< co::IModulePartLoader* >( loader_ );
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 1 ), range );
	}

	bool isLoadable( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::Range<co::Any> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 2 ), range );
		return res.get< bool >();
	}

	co::IModule* load( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::Range<co::Any> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 3 ), range );
		return res.get< co::IModule* >();
	}

	void uninstallLoader( co::IModulePartLoader* loader_ )
	{
		co::Any args[1];
		args[0].set< co::IModulePartLoader* >( loader_ );
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 4 ), range );
	}

protected:
	template<typename T>
	co::IField* getField( co::uint32 index )
	{
		return co::typeOf<T>::get()->getFields()[index];
	}

	template<typename T>
	co::IMethod* getMethod( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMethods()[index];
	}

private:
	co::IDynamicServiceProvider* _provider;
	co::uint32 _cookie;
};

// ------ Reflector Component ------ //

class IModuleManager_Reflector : public co::ReflectorBase
{
public:
	IModuleManager_Reflector()
	{
		// empty
	}

	virtual ~IModuleManager_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModuleManager>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IModuleManager_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IModuleManager* p = co::checkInstance<co::IModuleManager>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< bool >( p->getBinaryCompatibilityChecking() ); break;
		case 1:		value.set< co::Range<co::IModulePartLoader*> >( p->getLoaders() ); break;
		case 2:		value.set< co::Range<co::IModule*> >( p->getModules() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IModuleManager* p = co::checkInstance<co::IModuleManager>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		p->setBinaryCompatibilityChecking( value.get< bool >() ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::IModuleManager* p = co::checkInstance<co::IModuleManager>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 3:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IModule* >( p->findModule( moduleName_ ) );
				}
				break;
			case 4:
				{
					co::IModulePartLoader* loader_ = args[++argIndex].get< co::IModulePartLoader* >();
					argIndex = -1;
					p->installLoader( loader_ );
				}
				break;
			case 5:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< bool >( p->isLoadable( moduleName_ ) );
				}
				break;
			case 6:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IModule* >( p->load( moduleName_ ) );
				}
				break;
			case 7:
				{
					co::IModulePartLoader* loader_ = args[++argIndex].get< co::IModulePartLoader* >();
					argIndex = -1;
					p->uninstallLoader( loader_ );
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
			raiseArgumentTypeException( method, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIModuleManagerReflector()
{
    return new IModuleManager_Reflector;
}

} // namespace co
