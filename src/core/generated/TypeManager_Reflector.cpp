/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/TypeManager.h>
#include <co/DynamicProxyHandler.h>
#include <co/CSLError.h>
#include <co/Namespace.h>
#include <co/ArrayType.h>
#include <co/Type.h>
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

	// co.TypeManager Methods:

	bool getDocumentationParsing()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeManager>( 0 ), res );
		assert( res.containsObject() == false );
        return res.get< bool >();
	}

	void setDocumentationParsing( bool __documentationParsing )
	{
		co::Any arg;
		arg.set< bool >( __documentationParsing );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::TypeManager>( 0 ), arg );
	}

	co::Namespace* getRootNS()
	{
		co::Any res;
		_handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeManager>( 1 ), res );
		assert( res.containsObject() == false );
        return res.get< co::Namespace* >();
	}

	co::Namespace* findNamespace( const std::string& __fullName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __fullName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 2 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Namespace* >();
	}

	co::Type* findType( const std::string& __fullName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __fullName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 3 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Type* >();
	}

	co::ArrayType* getArrayOf( co::Type* __elementType )
	{
		co::Any res, args[1];
		args[0].set< co::Type* >( __elementType );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 4 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::ArrayType* >();
	}

	const std::string& getDocumentation( const std::string& __typeOrMemberName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __typeOrMemberName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 5 ), range, res );
		assert( res.containsObject() == false );
		return res.get< const std::string& >();
	}

	co::Type* getType( const std::string& __typeName )
	{
		co::Any res, args[1];
		args[0].set< const std::string& >( __typeName );
		co::ArrayRange<co::Any const> range( args, 1 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 6 ), range, res );
		assert( res.containsObject() == false );
		return res.get< co::Type* >();
	}

	co::Type* loadType( const std::string& __typeName, std::vector<co::CSLError>* __errorStack )
	{
		co::Any res, args[2];
		args[0].set< const std::string& >( __typeName );
		args[1].set< std::vector<co::CSLError>* >( __errorStack );
		co::ArrayRange<co::Any const> range( args, 2 );
		_handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 7 ), range, res );
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
		case 0:		value.set< bool >( p->getDocumentationParsing() ); break;
		case 1:		value.set< co::Namespace* >( p->getRootNS() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::TypeManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->setDocumentationParsing( value.get< bool >() ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::TypeManager* p = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 2:
				{
					const std::string& __fullName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Namespace* >( p->findNamespace( __fullName ) );
				}
				break;
			case 3:
				{
					const std::string& __fullName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Type* >( p->findType( __fullName ) );
				}
				break;
			case 4:
				{
					co::Type* __elementType = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					res.set< co::ArrayType* >( p->getArrayOf( __elementType ) );
				}
				break;
			case 5:
				{
					const std::string& __typeOrMemberName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< const std::string& >( p->getDocumentation( __typeOrMemberName ) );
				}
				break;
			case 6:
				{
					const std::string& __typeName = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Type* >( p->getType( __typeName ) );
				}
				break;
			case 7:
				{
					const std::string& __typeName = args[++argIndex].get< const std::string& >();
					std::vector<co::CSLError>* __errorStack = args[++argIndex].get< std::vector<co::CSLError>* >();
					argIndex = -1;
					res.set< co::Type* >( p->loadType( __typeName, __errorStack ) );
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

		return dynamic_cast<co::TypeManager*>( instance.getState().data.itf );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeManagerReflector()
{
    return new TypeManager_Reflector;
}

} // namespace co
