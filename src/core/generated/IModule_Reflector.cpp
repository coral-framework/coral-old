/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModule.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IModulePart.h>
#include <co/INamespace.h>
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

class IModule_Proxy : public co::IModule
{
public:
	IModule_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IModule_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IModule>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IModule Methods:

	co::INamespace* getNamespace()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IModule>( 0 ) );
        return res.get< co::INamespace* >();
	}

	co::Range<co::IModulePart*> getParts()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IModule>( 1 ) );
        return res.get< co::Range<co::IModulePart*> >();
	}

	co::int32 getRank()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IModule>( 2 ) );
        return res.get< co::int32 >();
	}

	void setRank( co::int32 rank_ )
	{
		co::Any arg( rank_ );
		_provider->dynamicSetField( _cookie, getField<co::IModule>( 2 ), arg );
	}

	co::ModuleState getState()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::IModule>( 3 ) );
        return res.get< co::ModuleState >();
	}

	void abort()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModule>( 0 ), range );
	}

	void disintegrate()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModule>( 1 ), range );
	}

	void dispose()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModule>( 2 ), range );
	}

	void initialize()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModule>( 3 ), range );
	}

	void integrate()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModule>( 4 ), range );
	}

	void integratePresentation()
	{
		co::Range<co::Any> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IModule>( 5 ), range );
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

class IModule_Reflector : public co::ReflectorBase
{
public:
	IModule_Reflector()
	{
		// empty
	}

	virtual ~IModule_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IModule>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IModule_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IModule* p = co::checkInstance<co::IModule>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->getNamespace(); break;
		case 1:		value = p->getParts(); break;
		case 2:		value = p->getRank(); break;
		case 3:		value = p->getState(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IModule* p = co::checkInstance<co::IModule>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		p->setRank( value.get< co::int32 >() ); break;
		case 3:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::IModule* p = co::checkInstance<co::IModule>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 4:
				{
					p->abort();
				}
				break;
			case 5:
				{
					p->disintegrate();
				}
				break;
			case 6:
				{
					p->dispose();
				}
				break;
			case 7:
				{
					p->initialize();
				}
				break;
			case 8:
				{
					p->integrate();
				}
				break;
			case 9:
				{
					p->integratePresentation();
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

co::IReflector* __createIModuleReflector()
{
    return new IModule_Reflector;
}

} // namespace co
