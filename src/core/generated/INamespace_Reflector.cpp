/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/INamespace.h>
#include <co/IDynamicServiceProvider.h>
#include <co/ITypeBuilder.h>
#include <co/IType.h>
#include <co/IModule.h>
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

	co::Range<co::INamespace*> getChildNamespaces()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 0 ) );
        return res.get< co::Range<co::INamespace*> >();
	}

	std::string getFullName()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 1 ) );
        return res.get< const std::string& >();
	}

	co::IModule* getModule()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 2 ) );
        return res.get< co::IModule* >();
	}

	std::string getName()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 3 ) );
        return res.get< const std::string& >();
	}

	co::INamespace* getParentNamespace()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 4 ) );
        return res.get< co::INamespace* >();
	}

	co::Range<co::IType*> getTypes()
	{
		co::AnyValue res = _provider->dynamicGetField( _cookie, getField<co::INamespace>( 5 ) );
        return res.get< co::Range<co::IType*> >();
	}

	co::INamespace* defineChildNamespace( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any> range( args, 1 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 0 ), range );
		return res.get< co::INamespace* >();
	}

	co::ITypeBuilder* defineType( const std::string& name_, co::TypeKind typeKind_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( name_ );
		args[1].set< co::TypeKind >( typeKind_ );
		co::Range<co::Any> range( args, 2 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 1 ), range );
		return res.get< co::ITypeBuilder* >();
	}

	co::INamespace* findChildNamespace( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any> range( args, 1 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 2 ), range );
		return res.get< co::INamespace* >();
	}

	co::IType* findType( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any> range( args, 1 );
		co::AnyValue res = _provider->dynamicInvoke( _cookie, getMethod<co::INamespace>( 3 ), range );
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

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::INamespace_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::INamespace* p = co::checkInstance<co::INamespace>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->getChildNamespaces(); break;
		case 1:		value = p->getFullName(); break;
		case 2:		value = p->getModule(); break;
		case 3:		value = p->getName(); break;
		case 4:		value = p->getParentNamespace(); break;
		case 5:		value = p->getTypes(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::INamespace* p = co::checkInstance<co::INamespace>( instance, field );
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

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any> args, co::AnyValue& res )
	{
		co::INamespace* p = co::checkInstance<co::INamespace>( instance, method );
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
					res = p->defineChildNamespace( name_ );
				}
				break;
			case 7:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					co::TypeKind typeKind_ = args[++argIndex].get< co::TypeKind >();
					argIndex = -1;
					res = p->defineType( name_, typeKind_ );
				}
				break;
			case 8:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res = p->findChildNamespace( name_ );
				}
				break;
			case 9:
				{
					const std::string& name_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res = p->findType( name_ );
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

co::IReflector* __createINamespaceReflector()
{
    return new INamespace_Reflector;
}

} // namespace co
