/*
 * Coral - Lightweight C++ Component Framework
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
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~ITypeManager_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::ITypeManager>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.ITypeManager Methods:

	co::INamespace* getRootNS()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::ITypeManager>( 0 ) );
        return res.get< co::INamespace* >();
	}

	co::INamespace* findNamespace( const std::string& fullName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( fullName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeManager>( 0 ), range );
		return res.get< co::INamespace* >();
	}

	co::IType* findType( const std::string& fullName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( fullName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeManager>( 1 ), range );
		return res.get< co::IType* >();
	}

	co::IArray* getArrayOf( co::IType* elementType_ )
	{
		co::Any args[1];
		args[0].set< co::IType* >( elementType_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeManager>( 2 ), range );
		return res.get< co::IArray* >();
	}

	co::IType* getType( const std::string& typeName_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( typeName_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeManager>( 3 ), range );
		return res.get< co::IType* >();
	}

	co::IType* loadType( const std::string& typeName_, std::vector<co::CSLError>& errorStack_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( typeName_ );
		args[1].set< std::vector<co::CSLError>& >( errorStack_ );
		co::Range<co::Any const> range( args, 2 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeManager>( 4 ), range );
		return res.get< co::IType* >();
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

	co::uint32 getSize()
	{
		return sizeof(co::ITypeManager);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::ITypeManager_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::ITypeManager* p = co::checkInstance<co::ITypeManager>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::INamespace* >( p->getRootNS() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::ITypeManager* p = co::checkInstance<co::ITypeManager>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::ITypeManager* p = co::checkInstance<co::ITypeManager>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					const std::string& fullName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::INamespace* >( p->findNamespace( fullName_ ) );
				}
				break;
			case 2:
				{
					const std::string& fullName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IType* >( p->findType( fullName_ ) );
				}
				break;
			case 3:
				{
					co::IType* elementType_ = args[++argIndex].get< co::IType* >();
					argIndex = -1;
					res.set< co::IArray* >( p->getArrayOf( elementType_ ) );
				}
				break;
			case 4:
				{
					const std::string& typeName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IType* >( p->getType( typeName_ ) );
				}
				break;
			case 5:
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

co::IReflector* __createITypeManagerReflector()
{
    return new ITypeManager_Reflector;
}

} // namespace co
