/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Namespace.h>
#include <co/DynamicProxyHandler.h>
#include <co/Type.h>
#include <co/TypeBuilder.h>
#include <co/Module.h>
#include <co/TypeCreationTransaction.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class Namespace_Proxy : public co::Namespace
{
public:
	Namespace_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::Namespace>( this ) );
	}

	virtual ~Namespace_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::Namespace>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.Namespace Methods:

	co::ArrayRange<co::Namespace* const> getChildNamespaces()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Namespace>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::Namespace* const> >();
	}

	const std::string& getFullName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Namespace>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::Module* getModule()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Namespace>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Module* >();
	}

	const std::string& getName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Namespace>( 3 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::Namespace* getParentNamespace()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Namespace>( 4 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Namespace* >();
	}

	co::ArrayRange<co::Type* const> getTypes()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::Namespace>( 5 ), res );
		assert( res.containsObject() == false );
        return res.get< co::ArrayRange<co::Type* const> >();
	}

	co::Namespace* defineChildNamespace( const std::string& name_ )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( name_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Namespace>( 6 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Namespace* >();
	}

	co::TypeBuilder* defineType( const std::string& name_, co::TypeKind typeKind_, co::TypeCreationTransaction* transaction_ )
	{
		co::Any res, args[3];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::TypeKind >( typeKind_ );
		args[2].set< co::TypeCreationTransaction* >( transaction_ );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Namespace>( 7 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::TypeBuilder* >();
	}

	co::Namespace* getChildNamespace( const std::string& name_ )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( name_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Namespace>( 8 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Namespace* >();
	}

	co::Type* getType( const std::string& name_ )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( name_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::Namespace>( 9 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Type* >();
	}

protected:
	template<typename T>
	co::AttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::MethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::DynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ Reflector ------ //

class Namespace_Reflector : public co::ReflectorBase
{
public:
	Namespace_Reflector()
	{
		// empty
	}

	virtual ~Namespace_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::Namespace>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::Namespace);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::Namespace>( new co::Namespace_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Namespace* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::ArrayRange<co::Namespace* const> >( p->getChildNamespaces() ); break;
		case 1:		value.set< const std::string& >( p->getFullName() ); break;
		case 2:		value.set< co::Module* >( p->getModule() ); break;
		case 3:		value.set< const std::string& >( p->getName() ); break;
		case 4:		value.set< co::Namespace* >( p->getParentNamespace() ); break;
		case 5:		value.set< co::ArrayRange<co::Type* const> >( p->getTypes() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Namespace* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
		case 4:		raiseAttributeIsReadOnly( ai ); break;
		case 5:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::Namespace* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 6:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Namespace* >( p->defineChildNamespace( name_ ) );
				}
				break;
			case 7:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::TypeKind typeKind_ = args[++argIndex].get< co::TypeKind >();
					co::TypeCreationTransaction* transaction_ = args[++argIndex].get< co::TypeCreationTransaction* >();
					argIndex = -1;
					res.set< co::TypeBuilder* >( p->defineType( name_, typeKind_, transaction_ ) );
				}
				break;
			case 8:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Namespace* >( p->getChildNamespace( name_ ) );
				}
				break;
			case 9:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Type* >( p->getType( name_ ) );
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
	co::Namespace* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::Namespace>::get();

		co::Namespace* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::Namespace*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Namespace*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.Namespace" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createNamespaceReflector()
{
    return new Namespace_Reflector;
}

} // namespace co
