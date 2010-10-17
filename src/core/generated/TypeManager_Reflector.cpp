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
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeManager>( 0 ) );
        return res.get< bool >();
	}

	void setDocumentationParsing( bool documentationParsing_ )
	{
		co::Any arg;
		arg.set< bool >( documentationParsing_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::TypeManager>( 0 ), arg );
	}

	co::Namespace* getRootNS()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::TypeManager>( 1 ) );
        return res.get< co::Namespace* >();
	}

	co::Namespace* findNamespace( const std::string& fullName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( fullName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 0 ), range );
		return res.get< co::Namespace* >();
	}

	co::Type* findType( const std::string& fullName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( fullName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 1 ), range );
		return res.get< co::Type* >();
	}

	co::ArrayType* getArrayOf( co::Type* elementType_ )
	{
		co::Any args[1];
		args[0].set< co::Type* >( elementType_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 2 ), range );
		return res.get< co::ArrayType* >();
	}

	const std::string& getDocumentation( const std::string& typeOrMemberName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( typeOrMemberName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 3 ), range );
		return res.get< const std::string& >();
	}

	co::Type* getType( const std::string& typeName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( typeName_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 4 ), range );
		return res.get< co::Type* >();
	}

	co::Type* loadType( const std::string& typeName_, std::vector<co::CSLError>* errorStack_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( typeName_ );
		args[1].set< std::vector<co::CSLError>* >( errorStack_ );
		co::ArrayRange<co::Any const> range( args, 2 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::TypeManager>( 5 ), range );
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
					const std::string& fullName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Namespace* >( p->findNamespace( fullName_ ) );
				}
				break;
			case 3:
				{
					const std::string& fullName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Type* >( p->findType( fullName_ ) );
				}
				break;
			case 4:
				{
					co::Type* elementType_ = args[++argIndex].get< co::Type* >();
					argIndex = -1;
					res.set< co::ArrayType* >( p->getArrayOf( elementType_ ) );
				}
				break;
			case 5:
				{
					const std::string& typeOrMemberName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< const std::string& >( p->getDocumentation( typeOrMemberName_ ) );
				}
				break;
			case 6:
				{
					const std::string& typeName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::Type* >( p->getType( typeName_ ) );
				}
				break;
			case 7:
				{
					const std::string& typeName_ = args[++argIndex].get< const std::string& >();
					std::vector<co::CSLError>* errorStack_ = args[++argIndex].get< std::vector<co::CSLError>* >();
					argIndex = -1;
					res.set< co::Type* >( p->loadType( typeName_, errorStack_ ) );
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
	co::TypeManager* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::InterfaceType* myType = co::typeOf<co::TypeManager>::get();

		co::TypeManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::TypeManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::TypeManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.TypeManager" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createTypeManagerReflector()
{
    return new TypeManager_Reflector;
}

} // namespace co
