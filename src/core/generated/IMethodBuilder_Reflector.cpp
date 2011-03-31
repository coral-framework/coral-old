/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMethodBuilder.h>
#include <co/IDynamicServiceProvider.h>
#include <co/ITypeBuilder.h>
#include <co/IException.h>
#include <co/IType.h>
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

class IMethodBuilder_Proxy : public co::IMethodBuilder
{
public:
	IMethodBuilder_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( co::disambiguate<co::IService, co::IMethodBuilder>( this ) );
	}

	virtual ~IMethodBuilder_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IMethodBuilder>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IMethodBuilder Methods:

	const std::string& getMethodName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMethodBuilder>( 0 ) );
        return res.get< const std::string& >();
	}

	co::ITypeBuilder* getTypeBuilder()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMethodBuilder>( 1 ) );
        return res.get< co::ITypeBuilder* >();
	}

	void createMethod()
	{
		co::Range<co::Any const> range;
		_provider->dynamicInvoke( _cookie, getMethod<co::IMethodBuilder>( 0 ), range );
	}

	void defineException( co::IException* exceptionType_ )
	{
		co::Any args[1];
		args[0].set< co::IException* >( exceptionType_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IMethodBuilder>( 1 ), range );
	}

	void defineParameter( const std::string& name_, co::IType* type_, bool input_, bool output_ )
	{
		co::Any args[4];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::IType* >( type_ );
		args[2].set< bool >( input_ );
		args[3].set< bool >( output_ );
		co::Range<co::Any const> range( args, 4 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IMethodBuilder>( 2 ), range );
	}

	void defineReturnType( co::IType* type_ )
	{
		co::Any args[1];
		args[0].set< co::IType* >( type_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IMethodBuilder>( 3 ), range );
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

class IMethodBuilder_Reflector : public co::ReflectorBase
{
public:
	IMethodBuilder_Reflector()
	{
		// empty
	}

	virtual ~IMethodBuilder_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IMethodBuilder>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IMethodBuilder);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IMethodBuilder>( new co::IMethodBuilder_Proxy( provider ) );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::IMethodBuilder* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getMethodName() ); break;
		case 1:		value.set< co::ITypeBuilder* >( p->getTypeBuilder() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::IMethodBuilder* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::IMethodBuilder* p = checkInstance( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 2:
				{
					p->createMethod();
				}
				break;
			case 3:
				{
					co::IException* exceptionType_ = args[++argIndex].get< co::IException* >();
					argIndex = -1;
					p->defineException( exceptionType_ );
				}
				break;
			case 4:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::IType* type_ = args[++argIndex].get< co::IType* >();
					bool input_ = args[++argIndex].get< bool >();
					bool output_ = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineParameter( name_, type_, input_, output_ );
				}
				break;
			case 5:
				{
					co::IType* type_ = args[++argIndex].get< co::IType* >();
					argIndex = -1;
					p->defineReturnType( type_ );
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

private:
	co::IMethodBuilder* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IMethodBuilder>::get();

		co::IMethodBuilder* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IMethodBuilder*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IMethodBuilder*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IMethodBuilder" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIMethodBuilderReflector()
{
    return new IMethodBuilder_Reflector;
}

} // namespace co
