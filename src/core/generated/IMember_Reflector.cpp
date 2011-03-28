/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMember.h>
#include <co/IDynamicServiceProvider.h>
#include <co/ICompositeType.h>
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

class IMember_Proxy : public co::IMember
{
public:
	IMember_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->registerProxyInterface( co::disambiguate<co::IService, co::IMember>( this ) );
	}

	virtual ~IMember_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterfaceType() { return co::typeOf<co::IMember>::get(); }
	co::IObject* getInterfaceOwner() { return _provider->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _provider->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _provider->componentRetain(); }
	void componentRelease() { _provider->componentRelease(); }

	// co.IMember Methods:

	co::uint16 getIndex()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IMember>( 0 ) );
        return res.get< co::uint16 >();
	}

	co::MemberKind getKind()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IMember>( 1 ) );
        return res.get< co::MemberKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IMember>( 2 ) );
        return res.get< const std::string& >();
	}

	co::ICompositeType* getOwner()
	{
		const co::Any& res = _provider->handleGetAttribute( _cookie, getAttribInfo<co::IMember>( 3 ) );
        return res.get< co::ICompositeType* >();
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

class IMember_Reflector : public co::ReflectorBase
{
public:
	IMember_Reflector()
	{
		// empty
	}

	virtual ~IMember_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IMember>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IMember);
	}

	co::IService* newProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return co::disambiguate<co::IService, co::IMember>( new co::IMember_Proxy( provider ) );
	}

	void getAttribute( const co::Any& instance, co::IField* ai, co::Any& value )
	{
		co::IMember* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< co::uint16 >( p->getIndex() ); break;
		case 1:		value.set< co::MemberKind >( p->getKind() ); break;
		case 2:		value.set< const std::string& >( p->getName() ); break;
		case 3:		value.set< co::ICompositeType* >( p->getOwner() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IField* ai, const co::Any& value )
	{
		co::IMember* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		case 2:		raiseAttributeIsReadOnly( ai ); break;
		case 3:		raiseAttributeIsReadOnly( ai ); break;
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
	co::IMember* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IMember>::get();

		co::IMember* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IMember*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IMember*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IMember" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIMemberReflector()
{
    return new IMember_Reflector;
}

} // namespace co
