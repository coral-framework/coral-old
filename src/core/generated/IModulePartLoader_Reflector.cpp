/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModulePartLoader.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IModulePart.h>
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

class IModulePartLoader_Proxy : public co::IModulePartLoader
{
public:
	IModulePartLoader_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IModulePartLoader_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IModulePartLoader>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IModulePartLoader Methods:

	bool canLoadModulePart( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IModulePartLoader>( 0 ), range );
		return res.get< bool >();
	}

	co::IModulePart* loadModulePart( const std::string& moduleName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( moduleName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::IModulePartLoader>( 1 ), range );
		return res.get< co::IModulePart* >();
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

class IModulePartLoader_Reflector : public co::ReflectorBase
{
public:
	IModulePartLoader_Reflector()
	{
		// empty
	}

	virtual ~IModulePartLoader_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModulePartLoader>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IModulePartLoader);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IModulePartLoader_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::checkInstance<co::IModulePartLoader>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::checkInstance<co::IModulePartLoader>( instance, field );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::IModulePartLoader* p = co::checkInstance<co::IModulePartLoader>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 0:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< bool >( p->canLoadModulePart( moduleName_ ) );
				}
				break;
			case 1:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IModulePart* >( p->loadModulePart( moduleName_ ) );
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

co::IReflector* __createIModulePartLoaderReflector()
{
    return new IModulePartLoader_Reflector;
}

} // namespace co
