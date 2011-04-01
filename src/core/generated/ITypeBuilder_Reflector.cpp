/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/ITypeBuilder.h>
#include <co/IDynamicServiceProvider.h>
#include <co/INamespace.h>
#include <co/IInterface.h>
#include <co/IMethodBuilder.h>
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

class ITypeBuilder_Proxy : public co::ITypeBuilder
{
public:
	ITypeBuilder_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~ITypeBuilder_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::ITypeBuilder>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.ITypeBuilder Methods:

	co::TypeKind getKind()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::ITypeBuilder>( 0 ) );
        return res.get< co::TypeKind >();
	}

	co::INamespace* getNamespace()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::ITypeBuilder>( 1 ) );
        return res.get< co::INamespace* >();
	}

	const std::string& getTypeName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::ITypeBuilder>( 2 ) );
        return res.get< const std::string& >();
	}

	co::IType* createType()
	{
		co::Range<co::Any const> range;
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 0 ), range );
		return res.get< co::IType* >();
	}

	void defineField( const std::string& name_, co::IType* type_, bool isReadOnly_ )
	{
		co::Any args[3];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::IType* >( type_ );
		args[2].set< bool >( isReadOnly_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 1 ), range );
	}

	void defineIdentifier( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 2 ), range );
	}

	co::IMethodBuilder* defineMethod( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 3 ), range );
		return res.get< co::IMethodBuilder* >();
	}

	void defineNativeClass( const std::string& nativeHeader_, const std::string& nativeName_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( nativeHeader_ );
		args[1].set< const std::string& >( nativeName_ );
		co::Range<co::Any const> range( args, 2 );
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 4 ), range );
	}

	void definePort( const std::string& name_, co::IInterface* type_, bool isFacet_ )
	{
		co::Any args[3];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::IInterface* >( type_ );
		args[2].set< bool >( isFacet_ );
		co::Range<co::Any const> range( args, 3 );
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 5 ), range );
	}

	void defineSuperType( co::IType* superType_ )
	{
		co::Any args[1];
		args[0].set< co::IType* >( superType_ );
		co::Range<co::Any const> range( args, 1 );
		_provider->dynamicInvoke( _cookie, getMethod<co::ITypeBuilder>( 6 ), range );
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

class ITypeBuilder_Reflector : public co::ReflectorBase
{
public:
	ITypeBuilder_Reflector()
	{
		// empty
	}

	virtual ~ITypeBuilder_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::ITypeBuilder>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::ITypeBuilder);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::ITypeBuilder_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::ITypeBuilder* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::TypeKind >( p->getKind() ); break;
		case 1:		value.set< co::INamespace* >( p->getNamespace() ); break;
		case 2:		value.set< const std::string& >( p->getTypeName() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::ITypeBuilder* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::ITypeBuilder* p = checkInstance( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 3:
				{
					res.set< co::IType* >( p->createType() );
				}
				break;
			case 4:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::IType* type_ = args[++argIndex].get< co::IType* >();
					bool isReadOnly_ = args[++argIndex].get< bool >();
					argIndex = -1;
					p->defineField( name_, type_, isReadOnly_ );
				}
				break;
			case 5:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->defineIdentifier( name_ );
				}
				break;
			case 6:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IMethodBuilder* >( p->defineMethod( name_ ) );
				}
				break;
			case 7:
				{
					const std::string& nativeHeader_ = args[++argIndex].get< const std::string& >();
					const std::string& nativeName_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->defineNativeClass( nativeHeader_, nativeName_ );
				}
				break;
			case 8:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::IInterface* type_ = args[++argIndex].get< co::IInterface* >();
					bool isFacet_ = args[++argIndex].get< bool >();
					argIndex = -1;
					p->definePort( name_, type_, isFacet_ );
				}
				break;
			case 9:
				{
					co::IType* superType_ = args[++argIndex].get< co::IType* >();
					argIndex = -1;
					p->defineSuperType( superType_ );
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
	co::ITypeBuilder* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::ITypeBuilder>::get();

		co::ITypeBuilder* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::ITypeBuilder*>( any.getState().data.service ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::ITypeBuilder*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.ITypeBuilder" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createITypeBuilderReflector()
{
    return new ITypeBuilder_Reflector;
}

} // namespace co
