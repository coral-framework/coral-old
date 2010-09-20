/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/TypeBuilder.h>
#include <co/DynamicProxyHandler.h>
#include <co/MethodBuilder.h>
#include <co/InterfaceType.h>
#include <co/Type.h>
#include <co/Namespace.h>
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

class TypeBuilder_Proxy : public co::TypeBuilder
{
public:
	TypeBuilder_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::TypeBuilder>( this ) );
	}

	virtual ~TypeBuilder_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::TypeBuilder>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.TypeBuilder Methods:

	co::TypeKind getKind()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeBuilder>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< co::TypeKind >();
	}

	co::Namespace* getNamespace()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeBuilder>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Namespace* >();
	}

	const std::string& getTypeName()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeBuilder>( 2 ), res );
		assert( res.containsObject() == false );
        return res.get< const std::string& >();
	}

	co::Type* createType()
	{
		co::Any res;
		co::ArrayRange<co::Any const> range;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 3 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Type* >();
	}

	void defineAttribute( const std::string& __name, co::Type* __type, bool __isReadOnly )
	{
		co::Any res, args[3];
		args[0].set< const std::string& >( __name );
		args[1].set< co::Type* >( __type );
		args[2].set< bool >( __isReadOnly );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 4 ), range, res );
	}

	void defineIdentifier( const std::string& __name )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __name );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 5 ), range, res );
	}

	void defineInterface( const std::string& __name, co::InterfaceType* __interfaceType, bool __isProvided )
	{
		co::Any res, args[3];
		args[0].set< const std::string& >( __name );
		args[1].set< co::InterfaceType* >( __interfaceType );
		args[2].set< bool >( __isProvided );
		co::ArrayRange<co::Any const> range( args, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 6 ), range, res );
	}

	co::MethodBuilder* defineMethod( const std::string& __name )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __name );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 7 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::MethodBuilder* >();
	}

	void defineNativeClass( const std::string& __nativeHeaderFile, const std::string& __nativeName )
	{
		co::Any res, args[2];
		args[0].set< const std::string& >( __nativeHeaderFile );
		args[1].set< const std::string& >( __nativeName );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 8 ), range, res );
	}

	void defineSuperType( co::Type* __superType )
	{
		co::Any res, args[1];
		args[0].set< co::Type* >( __superType );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 9 ), range, res );
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

class TypeBuilder_Reflector : public co::ReflectorBase
{
public:
	TypeBuilder_Reflector()
	{
		// empty
	}

	virtual ~TypeBuilder_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::TypeBuilder>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeBuilder);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::TypeBuilder>( new co::TypeBuilder_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::TypeBuilder* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::TypeKind >( p->getKind() ); break;
		case 1:		value.set< co::Namespace* >( p->getNamespace() ); break;
		case 2:		value.set< const std::string& >( p->getTypeName() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::TypeBuilder* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::TypeBuilder* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 3:
				{
					res.set< co::Type* >( p->createType() );
				}
				break;
			case 4:
				{
					const std::string& __name = args[++argIndex].get< const std::string& >();
					co::Type* __type = args[++argIndex].get< co::Type* >();
					bool __isReadOnly = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineAttribute( __name, __type, __isReadOnly );
				}
				break;
			case 5:
				{
					const std::string& __name = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->defineIdentifier( __name );
				}
				break;
			case 6:
				{
					const std::string& __name = args[++argIndex].get< const std::string& >();
					co::InterfaceType* __interfaceType = args[++argIndex].get< co::InterfaceType* >();
					bool __isProvided = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineInterface( __name, __interfaceType, __isProvided );
				}
				break;
			case 7:
				{
					const std::string& __name = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::MethodBuilder* >( p->defineMethod( __name ) );
				}
				break;
			case 8:
				{
					const std::string& __nativeHeaderFile = args[++argIndex].get< const std::string& >();
					const std::string& __nativeName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->defineNativeClass( __nativeHeaderFile, __nativeName );
				}
				break;
			case 9:
				{
					co::Type* __superType = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					p->defineSuperType( __superType );
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
	co::TypeBuilder* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::TypeBuilder>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::TypeBuilder*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return dynamic_cast<co::TypeBuilder*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeBuilderReflector()
{
    return new TypeBuilder_Reflector;
}

} // namespace co
