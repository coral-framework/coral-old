/*******************************************************************************
** Reflection code generated for type 'co.TypeBuilder'
**
** Created: Mon Jun 28 12:01:36 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/TypeBuilder.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/InterfaceType.h>
#include <co/MethodBuilder.h>
#include <co/Namespace.h>
#include <co/Type.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>

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
		// empty destructor
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::TypeBuilder>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::TypeBuilder Methods:

	co::TypeKind getKind()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeBuilder>( 0 ), __res );
        return __res.get< co::TypeKind >();
	}

	co::Namespace* getNamespace()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeBuilder>( 1 ), __res );
        return __res.get< co::Namespace* >();
	}

	const std::string& getTypeName()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeBuilder>( 2 ), __res );
        return __res.get< const std::string& >();
	}

	co::Type* createType( )
	{
		co::Any __res;
		co::ArrayRange<co::Any const> __ar;
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 3 ), __ar, __res );
		return __res.get< co::Type* >();
	}

	void defineAttribute( const std::string& name, co::Type* type, bool isReadOnly )
	{
		co::Any __res, __arg[3];
		__arg[0].set< const std::string& >( name );
		__arg[1].set< co::Type* >( type );
		__arg[2].set< bool >( isReadOnly );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 4 ), __ar, __res );
	}

	void defineIdentifier( const std::string& name )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( name );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 5 ), __ar, __res );
	}

	void defineInterface( const std::string& name, co::InterfaceType* interfaceType, bool isProvided )
	{
		co::Any __res, __arg[3];
		__arg[0].set< const std::string& >( name );
		__arg[1].set< co::InterfaceType* >( interfaceType );
		__arg[2].set< bool >( isProvided );
		co::ArrayRange<co::Any const> __ar( __arg, 3 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 6 ), __ar, __res );
	}

	co::MethodBuilder* defineMethod( const std::string& name )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( name );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 7 ), __ar, __res );
		return __res.get< co::MethodBuilder* >();
	}

	void defineNativeClass( const std::string& nativeName, const std::string& headerName )
	{
		co::Any __res, __arg[2];
		__arg[0].set< const std::string& >( nativeName );
		__arg[1].set< const std::string& >( headerName );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 8 ), __ar, __res );
	}

	void defineSuperType( co::Type* superType )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::Type* >( superType );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeBuilder>( 9 ), __ar, __res );
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
		return co::disambiguate<co::Interface, co::TypeBuilder>( new TypeBuilder_Proxy( handler ) );
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

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
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
					__res.set< co::Type* >( p->createType() );
				}
				break;
			case 4:
				{
					const std::string& name = args[++argIndex].get< const std::string& >();
					co::Type* type = args[++argIndex].get< co::Type* >();
					bool isReadOnly = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineAttribute( name, type, isReadOnly );
				}
				break;
			case 5:
				{
					const std::string& name = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->defineIdentifier( name );
				}
				break;
			case 6:
				{
					const std::string& name = args[++argIndex].get< const std::string& >();
					co::InterfaceType* interfaceType = args[++argIndex].get< co::InterfaceType* >();
					bool isProvided = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineInterface( name, interfaceType, isProvided );
				}
				break;
			case 7:
				{
					const std::string& name = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< co::MethodBuilder* >( p->defineMethod( name ) );
				}
				break;
			case 8:
				{
					const std::string& nativeName = args[++argIndex].get< const std::string& >();
					const std::string& headerName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->defineNativeClass( nativeName, headerName );
				}
				break;
			case 9:
				{
					co::Type* superType = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					p->defineSuperType( superType );
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
		CORAL_UNUSED( __res );
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

		return reinterpret_cast<co::TypeBuilder*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

namespace __coral {

co::Reflector* createTypeBuilderReflector()
{
    return new TypeBuilder_Reflector;
}

} // namespace __coral
