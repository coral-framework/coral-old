/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModuleManager.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IModulePartLoader.h>
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
		bool res;
		_provider->dynamicGetField( _cookie, getField<co::IModuleManager>( 0 ), res );
		return res;
	}

	void setBinaryCompatibilityChecking( bool binaryCompatibilityChecking_ )
	{
		_provider->dynamicSetField( _cookie, getField<co::IModuleManager>( 0 ), binaryCompatibilityChecking_ );
	}

	co::TSlice<co::IModulePartLoader*> getLoaders()
	{
		typedef co::Temporary<std::vector<co::IModulePartLoaderRef> > Temporary;
		std::unique_ptr<Temporary> temp( new Temporary );
		auto& res = temp->value;
		_provider->dynamicGetField( _cookie, getField<co::IModuleManager>( 1 ), res );
		return co::TSlice<co::IModulePartLoader*>( res, temp.release() );
	}

	co::TSlice<co::IModule*> getModules()
	{
		typedef co::Temporary<std::vector<co::IModuleRef> > Temporary;
		std::unique_ptr<Temporary> temp( new Temporary );
		auto& res = temp->value;
		_provider->dynamicGetField( _cookie, getField<co::IModuleManager>( 2 ), res );
		return co::TSlice<co::IModule*>( res, temp.release() );
	}

	co::IModule* findModule( const std::string& moduleName_ )
	{
		co::Any args[] = { moduleName_ };
		co::IModuleRef res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 0 ), args, res );
		return res.get();
	}

	void installLoader( co::IModulePartLoader* loader_ )
	{
		co::Any args[] = { loader_ };
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 1 ), args, co::Any() );
	}

	bool isLoadable( const std::string& moduleName_ )
	{
		co::Any args[] = { moduleName_ };
		bool res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 2 ), args, res );
		return res;
	}

	co::IModule* load( const std::string& moduleName_ )
	{
		co::Any args[] = { moduleName_ };
		co::IModuleRef res;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 3 ), args, res );
		return res.get();
	}

	void uninstallLoader( co::IModulePartLoader* loader_ )
	{
		co::Any args[] = { loader_ };
		_provider->dynamicInvoke( _cookie, getMethod<co::IModuleManager>( 4 ), args, co::Any() );
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

	void getField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::IModuleManager* p = co::checkInstance<co::IModuleManager>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.put( p->getBinaryCompatibilityChecking() ); break;
		case 1:		value.put( p->getLoaders() ); break;
		case 2:		value.put( p->getModules() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
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

	void invoke( const co::Any& instance, co::IMethod* method, co::Slice<co::Any> args, const co::Any& res )
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
					res.put( p->findModule( moduleName_ ) );
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
					res.put( p->isLoadable( moduleName_ ) );
				}
				break;
			case 6:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.put( p->load( moduleName_ ) );
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
