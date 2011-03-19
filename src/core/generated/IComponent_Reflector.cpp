/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IComponent.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IInterfaceInfo.h>
#include <co/IComponentType.h>
#include <co/IMethodInfo.h>
#include <co/IAttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class IComponent_Proxy : public co::IComponent
{
public:
	IComponent_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::IComponent>( this ) );
	}

	virtual ~IComponent_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::IComponent>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.IComponent Methods:

	co::IComponentType* getComponentType()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IComponent>( 0 ) );
        return res.get< co::IComponentType* >();
	}

	co::Interface* getInterface( co::IInterfaceInfo* itfInfo_ )
	{
		co::Any args[1];
		args[0].set< co::IInterfaceInfo* >( itfInfo_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::IComponent>( 0 ), range );
		return res.get< co::Interface* >();
	}

	void setReceptacle( co::IInterfaceInfo* receptacle_, co::Interface* facet_ )
	{
		co::Any args[2];
		args[0].set< co::IInterfaceInfo* >( receptacle_ );
		args[1].set< co::Interface* >( facet_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::IComponent>( 1 ), range );
	}

protected:
	template<typename T>
	co::IAttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::IMethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::IDynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ IReflector ------ //

class IComponent_Reflector : public co::ReflectorBase
{
public:
	IComponent_Reflector()
	{
		// empty
	}

	virtual ~IComponent_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IComponent>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IComponent);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::IComponent>( new co::IComponent_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::IComponent* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::IComponentType* >( p->getComponentType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::IComponent* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::IComponent* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					co::IInterfaceInfo* itfInfo_ = args[++argIndex].get< co::IInterfaceInfo* >();
					argIndex = -1;
					res.set< co::Interface* >( p->getInterface( itfInfo_ ) );
				}
				break;
			case 2:
				{
					co::IInterfaceInfo* receptacle_ = args[++argIndex].get< co::IInterfaceInfo* >();
					co::Interface* facet_ = args[++argIndex].get< co::Interface* >();
					argIndex = -1;
					p->setReceptacle( receptacle_, facet_ );
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
	co::IComponent* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::IComponent>::get();

		co::IComponent* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IComponent*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IComponent*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IComponent" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createIComponentIReflector()
{
    return new IComponent_Reflector;
}

} // namespace co
