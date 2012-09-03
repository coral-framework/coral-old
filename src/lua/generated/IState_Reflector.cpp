/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <lua/IState.h>
#include <co/IDynamicServiceProvider.h>
#include <lua/IInterceptor.h>
#include <co/IMethod.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace lua {

void moduleRetain();
void moduleRelease();

// ------ Dynamic Service Proxy ------ //

class IState_Proxy : public lua::IState
{
public:
	IState_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		moduleRetain();
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IState_Proxy()
	{
		moduleRelease();
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<lua::IState>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// lua.IState Methods:

	co::Range<lua::IInterceptor*> getInterceptors()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<lua::IState>( 0 ) );
        return res.get< co::Range<lua::IInterceptor*> >();
	}

	void addInterceptor( lua::IInterceptor* interceptor_ )
	{
		co::Any args[1];
		args[0].set< lua::IInterceptor* >( interceptor_ );
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 0 ), range );
	}

	co::int32 callFunction( const std::string& moduleName_, const std::string& functionName_, co::Range<co::Any> args_, co::Range<co::Any> results_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( moduleName_ );
		args[1].set< const std::string& >( functionName_ );
		args[2].set< co::Range<co::Any> >( args_ );
		args[3].set< co::Range<co::Any> >( results_ );
		co::Range<co::Any> range( args, 4 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 1 ), range );
		return res.get< co::int32 >();
	}

	void collectGarbage()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 2 ), range );
	}

	bool findScript( const std::string& name_, std::string& filename_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( name_ );
		args[1].set< std::string& >( filename_ );
		co::Range<co::Any> range( args, 2 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 3 ), range );
		return res.get< bool >();
	}

	void removeInterceptor( lua::IInterceptor* interceptor_ )
	{
		co::Any args[1];
		args[0].set< lua::IInterceptor* >( interceptor_ );
		co::Range<co::Any> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<lua::IState>( 4 ), range );
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

class IState_Reflector : public co::ReflectorBase
{
public:
	IState_Reflector()
	{
		moduleRetain();
	}

	virtual ~IState_Reflector()
	{
		moduleRelease();
	}

	co::IType* getType()
	{
		return co::typeOf<lua::IState>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new lua::IState_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		lua::IState* p = co::checkInstance<lua::IState>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->getInterceptors(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		lua::IState* p = co::checkInstance<lua::IState>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		lua::IState* p = co::checkInstance<lua::IState>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					lua::IInterceptor* interceptor_ = args[++argIndex].get< lua::IInterceptor* >();
					argIndex = -1;
					p->addInterceptor( interceptor_ );
				}
				break;
			case 2:
				{
					const std::string& moduleName_ = args[++argIndex].get< const std::string& >();
					const std::string& functionName_ = args[++argIndex].get< const std::string& >();
					co::Range<co::Any> args_ = args[++argIndex].get< co::Range<co::Any> >();
					co::Range<co::Any> results_ = args[++argIndex].get< co::Range<co::Any> >();
					argIndex = -1;
					res = p->callFunction( moduleName_, functionName_, args_, results_ );
				}
				break;
			case 3:
				{
					p->collectGarbage();
				}
				break;
			case 4:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					std::string& filename_ = args[++argIndex].get< std::string& >();
					argIndex = -1;
					res = p->findScript( name_, filename_ );
				}
				break;
			case 5:
				{
					lua::IInterceptor* interceptor_ = args[++argIndex].get< lua::IInterceptor* >();
					argIndex = -1;
					p->removeInterceptor( interceptor_ );
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

co::IReflector* __createIStateReflector()
{
    return new IState_Reflector;
}

} // namespace lua
