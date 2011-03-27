/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IModule.h>
#include <co/IDynamicServiceProvider.h>
#include <co/INamespace.h>
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

class IModule_Proxy : public co::IModule
{
public:
	IModule_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::IModule>( this ) );
	}

	virtual ~IModule_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::IModule>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IModule Methods:

	co::INamespace* getNamespace()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 0 ) );
        return res.get< co::INamespace* >();
	}

	co::Range<co::IModulePart* const> getParts()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 1 ) );
        return res.get< co::Range<co::IModulePart* const> >();
	}

	co::int32 getRank()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 2 ) );
        return res.get< co::int32 >();
	}

	void setRank( co::int32 rank_ )
	{
		co::Any arg;
		arg.set< co::int32 >( rank_ );
		_provider->handleSetAttribute( _cookie, getAttribInfo<co::IModule>( 2 ), arg );
	}

	co::ModuleState getState()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IModule>( 3 ) );
        return res.get< co::ModuleState >();
	}

	void abort()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 0 ), range );
	}

	void disintegrate()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 1 ), range );
	}

	void dispose()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 2 ), range );
	}

	void initialize()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 3 ), range );
	}

	void integrate()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 4 ), range );
	}

	void integratePresentation()
	{
		co::Range<co::Any const> range;
		_provider->handleMethodInvocation( _cookie, getMethodInfo<co::IModule>( 5 ), range );
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

	co::int32 getSize()
	{
		return sizeof(co::IModule);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IModule>( new co::IModule_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::IModule* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::INamespace* >( p->getNamespace() ); break;
		case 1:		value.set< co::Range<co::IModulePart* const> >( p->getParts() ); break;
		case 2:		value.set< co::int32 >( p->getRank() ); break;
		case 3:		value.set< co::ModuleState >( p->getState() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::IModule* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		p->setRank( value.get< co::int32 >() ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		co::IModule* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
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
			raiseArgumentTypeException( mi, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
	}

private:
	co::IModule* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IModule>::get();

		co::IModule* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IModule*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IModule*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IModule" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIModuleReflector()
{
    return new IModule_Reflector;
}

} // namespace co
