/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/IDocumentation.h>
#include <co/IDynamicServiceProvider.h>
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

class IDocumentation_Proxy : public co::IDocumentation
{
public:
	IDocumentation_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IDocumentation_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IDocumentation>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IAnnotation Methods:

	// co.IDocumentation Methods:

	const std::string& getValue()
	{
		co::Any res = _provider->dynamicGetField( _cookie, getField<co::IDocumentation>( 0 ) );
        return res.get< const std::string& >();
	}

	void setValue( const std::string& value_ )
	{
		co::Any arg;
		arg.set< const std::string& >( value_ );
		_provider->dynamicSetField( _cookie, getField<co::IDocumentation>( 0 ), arg );
	}

	void addDocFor( const std::string& element_, const std::string& text_ )
	{
		co::Any args[2];
		args[0].set< const std::string& >( element_ );
		args[1].set< const std::string& >( text_ );
		co::Range<co::Any const> range( args, 2 );
		_provider->dynamicInvoke( _cookie, getMethod<co::IDocumentation>( 0 ), range );
	}

	const std::string& getDocFor( const std::string& element_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( element_ );
		co::Range<co::Any const> range( args, 1 );
		co::Any res = _provider->dynamicInvoke( _cookie, getMethod<co::IDocumentation>( 1 ), range );
		return res.get< const std::string& >();
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

class IDocumentation_Reflector : public co::ReflectorBase
{
public:
	IDocumentation_Reflector()
	{
		// empty
	}

	virtual ~IDocumentation_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IDocumentation>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(void*);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IDocumentation_Proxy( provider );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::IDocumentation* p = co::checkInstance<co::IDocumentation>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getValue() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
	{
		co::IDocumentation* p = co::checkInstance<co::IDocumentation>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		p->setValue( value.get< const std::string& >() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invoke( co::Any instance, co::IMethod* method, co::Range<co::Any const> args, co::AnyValue& res )
	{
		co::IDocumentation* p = co::checkInstance<co::IDocumentation>( instance, method );
		checkNumArguments( method, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( method->getIndex() )
			{
			case 1:
				{
					const std::string& element_ = args[++argIndex].get< const std::string& >();
					const std::string& text_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					p->addDocFor( element_, text_ );
				}
				break;
			case 2:
				{
					const std::string& element_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					res.set< const std::string& >( p->getDocFor( element_ ) );
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

co::IReflector* __createIDocumentationReflector()
{
    return new IDocumentation_Reflector;
}

} // namespace co
