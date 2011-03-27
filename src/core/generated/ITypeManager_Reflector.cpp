/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ITypeManager.h>
#include <co/IDynamicServiceProvider.h>
#include <co/CSLError.h>
#include <co/INamespace.h>
#include <co/IType.h>
#include <co/IArray.h>
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

class ITypeManager_Proxy : public co::ITypeManager
{
public:
	ITypeManager_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::ITypeManager>( this ) );
	}

	virtual ~ITypeManager_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::ITypeManager>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.ITypeManager Methods:

	bool getDocumentationParsing()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ITypeManager>( 0 ) );
        return res.get< bool >();
	}

	void setDocumentationParsing( bool documentationParsing_ )
	{
		co::Any arg;
		arg.set< bool >( documentationParsing_ );
		_provider->handleSetAttribute( _cookie, getAttribInfo<co::ITypeManager>( 0 ), arg );
	}

	co::INamespace* getRootNS()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ITypeManager>( 1 ) );
        return res.get< co::INamespace* >();
	}

	co::INamespace* findNamespace( const std::string& fullName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( fullName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeManager>( 0 ), range );
		return res.get< co::INamespace* >();
	}

	co::IType* findType( const std::string& fullName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( fullName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeManager>( 1 ), range );
		return res.get< co::IType* >();
	}

	co::IArray* getArrayOf( co::IType* elementType_ )
	{
		co::Any args[1];
		args[0].set< co::IType* >( elementType_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeManager>( 2 ), range );
		return res.get< co::IArray* >();
	}

	const std::string& getDocumentation( const std::string& typeOrMemberName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( typeOrMemberName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeManager>( 3 ), range );
		return res.get< const std::string& >();
	}

	co::IType* getType( const std::string& typeName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( typeName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeManager>( 4 ), range );
		return res.get< co::IType* >();
	}

	co::IType* loadType( const std::string& typeName_, std::vector<co::CSLError>& errorStack_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( typeName_ );
		args[1].set< std::vector<co::CSLError>& >( errorStack_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ITypeManager>( 5 ), range );
		return res.get< co::IType* >();
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

class ITypeManager_Reflector : public co::ReflectorBase
{
public:
	ITypeManager_Reflector()
	{
		// empty
	}

	virtual ~ITypeManager_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ITypeManager>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ITypeManager);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::ITypeManager>( new co::ITypeManager_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::ITypeManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( p->getDocumentationParsing() ); break;
		case 1:		value.set< co::INamespace* >( p->getRootNS() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::ITypeManager* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->setDocumentationParsing( value.get< bool >() ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		co::ITypeManager* p = checkInstance( instance, mi );
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
					res.set< co::INamespace* >( p->findNamespace( fullName_ ) );
				}
				break;
			case 3:
				{
					const std::string& fullName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IType* >( p->findType( fullName_ ) );
				}
				break;
			case 4:
				{
					co::IType* elementType_ = args[++argIndex].get< co::IType* >();
					argIndex = -1;
					res.set< co::IArray* >( p->getArrayOf( elementType_ ) );
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
					res.set< co::IType* >( p->getType( typeName_ ) );
				}
				break;
			case 7:
				{
					const std::string& typeName_ = args[++argIndex].get< const std::string& >();
					std::vector<co::CSLError>& errorStack_ = args[++argIndex].get< std::vector<co::CSLError>& >();
					argIndex = -1;
					res.set< co::IType* >( p->loadType( typeName_, errorStack_ ) );
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
	co::ITypeManager* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::ITypeManager>::get();

		co::ITypeManager* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ITypeManager*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ITypeManager*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ITypeManager" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createITypeManagerReflector()
{
    return new ITypeManager_Reflector;
}

} // namespace co
