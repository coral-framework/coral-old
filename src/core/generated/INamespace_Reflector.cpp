/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/INamespace.h>
#include <co/IDynamicServiceProvider.h>
#include <co/ITypeTransaction.h>
#include <co/IType.h>
#include <co/IModule.h>
#include <co/ITypeBuilder.h>
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

class INamespace_Proxy : public co::INamespace
{
public:
	INamespace_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~INamespace_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::INamespace>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.INamespace Methods:

	co::Range<co::INamespace* const> getChildNamespaces()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 0 ) );
        return res.get< co::Range<co::INamespace* const> >();
	}

	const std::string& getFullName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 1 ) );
        return res.get< const std::string& >();
	}

	co::IModule* getModule()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 2 ) );
        return res.get< co::IModule* >();
	}

	const std::string& getName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 3 ) );
        return res.get< const std::string& >();
	}

	co::INamespace* getParentNamespace()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 4 ) );
        return res.get< co::INamespace* >();
	}

	co::Range<co::IType* const> getTypes()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 5 ) );
        return res.get< co::Range<co::IType* const> >();
	}

	co::INamespace* defineChildNamespace( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 0 ), range );
		return res.get< co::INamespace* >();
	}

	co::ITypeBuilder* defineType( const std::string& name_, co::TypeKind typeKind_, co::ITypeTransaction* transaction_ )
	{
		co::Any args[3];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::TypeKind >( typeKind_ );
		args[2].set< co::ITypeTransaction* >( transaction_ );
		co::Range<co::Any const> range( args, 3 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 1 ), range );
		return res.get< co::ITypeBuilder* >();
	}

	co::INamespace* getChildNamespace( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 2 ), range );
		return res.get< co::INamespace* >();
	}

	co::IType* getType( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 3 ), range );
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

class INamespace_Reflector : public co::ReflectorBase
{
public:
	INamespace_Reflector()
	{
		// empty
	}

	virtual ~INamespace_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::INamespace>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::INamespace);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::INamespace_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::INamespace* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::Range<co::INamespace* const> >( p->getChildNamespaces() ); break;
		case 1:		value.set< const std::string& >( p->getFullName() ); break;
		case 2:		value.set< co::IModule* >( p->getModule() ); break;
		case 3:		value.set< const std::string& >( p->getName() ); break;
		case 4:		value.set< co::INamespace* >( p->getParentNamespace() ); break;
		case 5:		value.set< co::Range<co::IType* const> >( p->getTypes() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::INamespace* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		raiseFieldIsReadOnly( field ); break;
		case 1:		raiseFieldIsReadOnly( field ); break;
		case 2:		raiseFieldIsReadOnly( field ); break;
		case 3:		raiseFieldIsReadOnly( field ); break;
		case 4:		raiseFieldIsReadOnly( field ); break;
		case 5:		raiseFieldIsReadOnly( field ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( const co::Any& instance, co::IMethod* method, co::Range<co::Any const> args, co::Any& res )
	{
		co::INamespace* p = checkInstance( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 6:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::INamespace* >( p->defineChildNamespace( name_ ) );
				}
				break;
			case 7:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::TypeKind typeKind_ = args[++argIndex].get< co::TypeKind >();
					co::ITypeTransaction* transaction_ = args[++argIndex].get< co::ITypeTransaction* >();
					argIndex = -1;
					res.set< co::ITypeBuilder* >( p->defineType( name_, typeKind_, transaction_ ) );
				}
				break;
			case 8:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::INamespace* >( p->getChildNamespace( name_ ) );
				}
				break;
			case 9:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< co::IType* >( p->getType( name_ ) );
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
	co::INamespace* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::INamespace>::get();

		co::INamespace* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::INamespace*>( any.getState().data.service ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::INamespace*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.INamespace" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createINamespaceReflector()
{
    return new INamespace_Reflector;
}

} // namespace co
