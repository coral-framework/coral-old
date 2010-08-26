/*******************************************************************************
** Reflection code generated for type 'co.TypeManager'
**
** Created: Wed Aug 25 16:31:31 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/TypeManager.h>
#include <co/reserved/ReflectorBase.h>
#include <co/DynamicProxyHandler.h>
#include <co/ArrayType.h>
#include <co/CSLError.h>
#include <co/Namespace.h>
#include <co/Type.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class TypeManager_Proxy : public co::TypeManager
{
public:
	TypeManager_Proxy( co::DynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::TypeManager>( this ) );
	}

	virtual ~TypeManager_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::InterfaceType* getInterfaceType() { return co::typeOf<co::TypeManager>::get(); }
	co::Component* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co::TypeManager Methods:

	co::Namespace* getRootNS()
	{
		co::Any __res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeManager>( 0 ), __res );
        return __res.get< co::Namespace* >();
	}

	co::Namespace* findNamespace( const std::string& fullName )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( fullName );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 1 ), __ar, __res );
		return __res.get< co::Namespace* >();
	}

	co::Type* findType( const std::string& fullName )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( fullName );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 2 ), __ar, __res );
		return __res.get< co::Type* >();
	}

	co::ArrayType* getArrayOf( co::Type* elementType )
	{
		co::Any __res, __arg[1];
		__arg[0].set< co::Type* >( elementType );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 3 ), __ar, __res );
		return __res.get< co::ArrayType* >();
	}

	co::Type* getType( const std::string& typeName )
	{
		co::Any __res, __arg[1];
		__arg[0].set< const std::string& >( typeName );
		co::ArrayRange<co::Any const> __ar( __arg, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 4 ), __ar, __res );
		return __res.get< co::Type* >();
	}

	co::Type* loadType( const std::string& typeName, std::vector<co::CSLError>* errorStack )
	{
		co::Any __res, __arg[2];
		__arg[0].set< const std::string& >( typeName );
		__arg[1].set< std::vector<co::CSLError>* >( errorStack );
		co::ArrayRange<co::Any const> __ar( __arg, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 5 ), __ar, __res );
		return __res.get< co::Type* >();
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

class TypeManager_Reflector : public co::ReflectorBase
{
public:
	TypeManager_Reflector()
	{
		// empty
	}

	virtual ~TypeManager_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::TypeManager>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::TypeManager);
	}

	co::Interface* newProxy( co::DynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::TypeManager>( new co::TypeManager_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::TypeManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::Namespace* >( p->getRootNS() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::TypeManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& __res )
	{
		co::TypeManager* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					const std::string& fullName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< co::Namespace* >( p->findNamespace( fullName ) );
				}
				break;
			case 2:
				{
					const std::string& fullName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< co::Type* >( p->findType( fullName ) );
				}
				break;
			case 3:
				{
					co::Type* elementType = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					__res.set< co::ArrayType* >( p->getArrayOf( elementType ) );
				}
				break;
			case 4:
				{
					const std::string& typeName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					__res.set< co::Type* >( p->getType( typeName ) );
				}
				break;
			case 5:
				{
					const std::string& typeName = args[++argIndex].get< const std::string& >();
					std::vector<co::CSLError>* errorStack = args[++argIndex].get< std::vector<co::CSLError>* >();
					argIndex = -1;
					__res.set< co::Type* >( p->loadType( typeName, errorStack ) );
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
	co::TypeManager* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::InterfaceType* myType = co::typeOf<co::TypeManager>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_INTERFACE ||
			!instance.getInterfaceType()->isSubTypeOf( myType ) ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::TypeManager*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::TypeManager*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeManagerReflector()
{
    return new TypeManager_Reflector;
}

} // namespace co
