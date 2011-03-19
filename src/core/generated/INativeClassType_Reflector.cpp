/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/INativeClassType.h>
#include <co/IDynamicProxyHandler.h>
#include <co/IMemberInfo.h>
#include <co/IAttributeInfo.h>
#include <co/INamespace.h>
#include <co/IReflector.h>
#include <co/IMethodInfo.h>
#include <co/Uuid.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Proxy Interface ------ //

class INativeClassType_Proxy : public co::INativeClassType
{
public:
	INativeClassType_Proxy( co::IDynamicProxyHandler* handler ) : _handler( handler )
	{
		_cookie = _handler->registerProxyInterface( co::disambiguate<co::Interface, co::INativeClassType>( this ) );
	}

	virtual ~INativeClassType_Proxy()
	{
		// empty
	}

	// co::Interface Methods:

	co::IInterfaceType* getInterfaceType() { return co::typeOf<co::INativeClassType>::get(); }
	co::IComponent* getInterfaceOwner() { return _handler->getInterfaceOwner(); }
	const std::string& getInterfaceName() { return _handler->getProxyInterfaceName( _cookie ); }
	void componentRetain() { _handler->componentRetain(); }
	void componentRelease() { _handler->componentRelease(); }

	// co.ICompoundType Methods:

	co::ArrayRange<co::IMemberInfo* const> getMembers()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::ICompoundType>( 0 ) );
        return res.get< co::ArrayRange<co::IMemberInfo* const> >();
	}

	co::IMemberInfo* getMember( const std::string& name_ )
	{
		co::Any args[1];
		args[0].set< const std::string& >( name_ );
		co::ArrayRange<co::Any const> range( args, 1 );
		const co::Any& res = _handler->handleMethodInvocation( _cookie, getMethodInfo<co::ICompoundType>( 0 ), range );
		return res.get< co::IMemberInfo* >();
	}

	// co.IAttributeContainer Methods:

	co::ArrayRange<co::IAttributeInfo* const> getMemberAttributes()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IAttributeContainer>( 0 ) );
        return res.get< co::ArrayRange<co::IAttributeInfo* const> >();
	}

	// co.IType Methods:

	const co::Uuid& getBinarySignature()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 0 ) );
        return res.get< const co::Uuid& >();
	}

	const std::string& getFullName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 1 ) );
        return res.get< const std::string& >();
	}

	const co::Uuid& getFullSignature()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 2 ) );
        return res.get< const co::Uuid& >();
	}

	co::TypeKind getKind()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 3 ) );
        return res.get< co::TypeKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 4 ) );
        return res.get< const std::string& >();
	}

	co::INamespace* getNamespace()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 5 ) );
        return res.get< co::INamespace* >();
	}

	co::IReflector* getReflector()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IType>( 6 ) );
        return res.get< co::IReflector* >();
	}

	void setReflector( co::IReflector* reflector_ )
	{
		co::Any arg;
		arg.set< co::IReflector* >( reflector_ );
		_handler->handleSetAttribute( _cookie, getAttribInfo<co::IType>( 6 ), arg );
	}

	// co.IMethodContainer Methods:

	co::ArrayRange<co::IMethodInfo* const> getMemberMethods()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::IMethodContainer>( 0 ) );
        return res.get< co::ArrayRange<co::IMethodInfo* const> >();
	}

	// co.INativeClassType Methods:

	const std::string& getNativeHeaderFile()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::INativeClassType>( 0 ) );
        return res.get< const std::string& >();
	}

	const std::string& getNativeName()
	{
		const co::Any& res = _handler->handleGetAttribute( _cookie, getAttribInfo<co::INativeClassType>( 1 ) );
        return res.get< const std::string& >();
	}

protected:
	template<typename T>
	co::IAttributeInfo* getAttribInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberAttributes()[index];
	}

	template<typename T>
	co::IMethodInfo* getMethodInfo( co::uint32 index )
	{
		return co::typeOf<T>::get()->getMemberMethods()[index];
	}

private:
	co::IDynamicProxyHandler* _handler;
	co::uint32 _cookie;
};

// ------ IReflector ------ //

class INativeClassType_Reflector : public co::ReflectorBase
{
public:
	INativeClassType_Reflector()
	{
		// empty
	}

	virtual ~INativeClassType_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::INativeClassType>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::INativeClassType);
	}

	co::Interface* newProxy( co::IDynamicProxyHandler* handler )
	{
		checValidProxyHandler( handler );
		return co::disambiguate<co::Interface, co::INativeClassType>( new co::INativeClassType_Proxy( handler ) );
	}

	void getAttribute( const co::Any& instance, co::IAttributeInfo* ai, co::Any& value )
	{
		co::INativeClassType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->getNativeHeaderFile() ); break;
		case 1:		value.set< const std::string& >( p->getNativeName() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::IAttributeInfo* ai, const co::Any& value )
	{
		co::INativeClassType* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		case 1:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( p );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::IMethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		checkInstance( instance, mi );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::INativeClassType* checkInstance( const co::Any& any, co::IMemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterfaceType* myType = co::typeOf<co::INativeClassType>::get();

		co::INativeClassType* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::INativeClassType*>( any.getState().data.itf ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::INativeClassType*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.INativeClassType" );

		return res;
	}
};

// ------ IReflector Creation Function ------ //

co::IReflector* __createINativeClassTypeIReflector()
{
    return new INativeClassType_Reflector;
}

} // namespace co
