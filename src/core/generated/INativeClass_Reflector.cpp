/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/INativeClass.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IField.h>
#include <co/IReflector.h>
#include <co/INamespace.h>
#include <co/IMember.h>
#include <co/Uuid.h>
#include <co/IMethod.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class INativeClass_Proxy : public co::INativeClass
{
public:
	INativeClass_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::INativeClass>( this ) );
	}

	virtual ~INativeClass_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::INativeClass>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IClassType Methods:

	co::Range<co::IMethod* const> getMethods()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IClassType>( 0 ) );
        return res.get< co::Range<co::IMethod* const> >();
	}

	// co.IRecordType Methods:

	co::Range<co::IField* const> getFields()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IRecordType>( 0 ) );
        return res.get< co::Range<co::IField* const> >();
	}

	// co.ICompositeType Methods:

	co::Range<co::IMember* const> getMembers()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::ICompositeType>( 0 ) );
        return res.get< co::Range<co::IMember* const> >();
	}

	co::IMember* getMember( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::Range<co::Any const> range( args, 1 );
		const co::Any& res = _provider->handleMethodInvocation( _cookie, getMethodInfo<co::ICompositeType>( 0 ), range );
		return res.get< co::IMember* >();
	}

	// co.IType Methods:

	const co::Uuid& getBinarySignature()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 0 ) );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 1 ) );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 2 ) );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 3 ) );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 4 ) );
        return res.get< const std::string& >();
	}

	co::INamespace* getNamespace()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 5 ) );
        return res.get< co::INamespace* >();
	}

	co::IReflector* getReflector()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 6 ) );
        return res.get< co::IReflector* >();
	}

	void setReflector( co::IReflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::IReflector* >( reflector_ );
		_provider->handleSetAttribute( _cookie, getAttribInfo<co::IType>( 6 ), arg );
	}

	// co.INativeClass Methods:

	const std::string& getNativeHeader()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::INativeClass>( 0 ) );
        return res.get< const std::string& >();
	}

	const std::string& getNativeName()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::INativeClass>( 1 ) );
        return res.get< const std::string& >();
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

class INativeClass_Reflector : public co::ReflectorBase
{
public:
	INativeClass_Reflector()
	{
		// empty
	}

	virtual ~INativeClass_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::INativeClass>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::INativeClass);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::INativeClass>( new co::INativeClass_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::INativeClass* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getNativeHeader() ); break;
		case 1:		value.set< const std::string& >( p->getNativeName() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::INativeClass* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethod* mi, co::Range<co::Any const> args, co::Any& res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::INativeClass* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::INativeClass>::get();

		co::INativeClass* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::INativeClass*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::INativeClass*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.INativeClass" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createINativeClassReflector()
{
    return new INativeClass_Reflector;
}

} // namespace co
