/*
 * Coral - Lightweight C++ Component Framework
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
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~ISystem_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::ISystem>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.ISystem Methods:

	co::IModuleManager* getModules()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::ISystem>( 0 ) );
        return res.get< co::IModuleManager* >();
	}

	co::IServiceManager* getServices()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::ISystem>( 1 ) );
        return res.get< co::IServiceManager* >();
	}

	co::SystemState getState()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::ISystem>( 2 ) );
        return res.get< co::SystemState >();
	}

	co::ITypeManager* getTypes()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::ISystem>( 3 ) );
        return res.get< co::ITypeManager* >();
	}

	void setupBase( co::Range<std::string> requiredModules_ )
	{
		co::Any args[1];
		args[0].set< co::Range<std::string> >( requiredModules_ );
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::ISystem>( 0 ), range );
	}

	void setupPresentation()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::ISystem>( 1 ), range );
	}

	void tearDown()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::ISystem>( 2 ), range );
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

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::ISystem_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::ISystem* p = co::checkInstance<co::ISystem>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->getModules(); break;
		case 1:		value = p->getServices(); break;
		case 2:		value = p->getState(); break;
		case 3:		value = p->getTypes(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::ISystem* p = co::checkInstance<co::ISystem>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		raiseFieldIsReadOnly( field ); break;
		case 3:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::ISystem* p = co::checkInstance<co::ISystem>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 4:
				{
					co::Range<std::string> requiredModules_ = args[++argIndex].get< co::Range<std::string> >();
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

co::IReflector* __createISystemReflector()
{
    return new ISystem_Reflector;
}

} // namespace co
