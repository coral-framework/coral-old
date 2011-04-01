/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/IMethod.h>
#include <co/IDynamicServiceProvider.h>
#include <co/IParameter.h>
#include <co/IException.h>
#include <co/ICompositeType.h>
#include <co/IType.h>
#include <co/IField.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Dynamic Service Proxy ------ //

class IMethod_Proxy : public co::IMethod
{
public:
	IMethod_Proxy( co::IDynamicServiceProvider* provider ) : _provider( provider )
	{
		_cookie = _provider->dynamicRegisterService( this );
	}

	virtual ~IMethod_Proxy()
	{
		// empty
	}

	// co::IService Methods:

	co::IInterface* getInterface() { return co::typeOf<co::IMethod>::get(); }
	co::IObject* getProvider() { return _provider->getProvider(); }
	co::IPort* getFacet() { return _provider->dynamicGetFacet( _cookie ); }
	void serviceRetain() { _provider->serviceRetain(); }
	void serviceRelease() { _provider->serviceRelease(); }

	// co.IMember Methods:

	co::uint16 getIndex()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 0 ) );
        return res.get< co::uint16 >();
	}

	co::MemberKind getKind()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 1 ) );
        return res.get< co::MemberKind >();
	}

	const std::string& getName()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 2 ) );
        return res.get< const std::string& >();
	}

	co::ICompositeType* getOwner()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMember>( 3 ) );
        return res.get< co::ICompositeType* >();
	}

	// co.IMethod Methods:

	co::Range<co::IException* const> getExceptions()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMethod>( 0 ) );
        return res.get< co::Range<co::IException* const> >();
	}

	co::Range<co::IParameter* const> getParameters()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMethod>( 1 ) );
        return res.get< co::Range<co::IParameter* const> >();
	}

	co::IType* getReturnType()
	{
		const co::Any& res = _provider->dynamicGetField( _cookie, getField<co::IMethod>( 2 ) );
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

class IMethod_Reflector : public co::ReflectorBase
{
public:
	IMethod_Reflector()
	{
		// empty
	}

	virtual ~IMethod_Reflector()
	{
		// empty
	}

	co::IType* getType()
	{
		return co::typeOf<co::IMethod>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::IMethod);
	}

	co::IService* newDynamicProxy( co::IDynamicServiceProvider* provider )
	{
		checkValidDynamicProvider( provider );
		return new co::IMethod_Proxy( provider );
	}

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::IMethod* p = checkInstance( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< co::Range<co::IException* const> >( p->getExceptions() ); break;
		case 1:		value.set< co::Range<co::IParameter* const> >( p->getParameters() ); break;
		case 2:		value.set< co::IType* >( p->getReturnType() ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::IMethod* p = checkInstance( instance, field );
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
		checkInstance( instance, method );
		raiseUnexpectedMemberIndex();
		CORAL_UNUSED( args );
		CORAL_UNUSED( res );
	}

private:
	co::IMethod* checkInstance( const co::Any& any, co::IMember* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::IInterface* myType = co::typeOf<co::IMethod>::get();

		co::IMethod* res;
		if( any.getKind() != co::TK_INTERFACE || !( res = dynamic_cast<co::IMethod*>( any.getState().data.service ) ) )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::IMethod*, but got " << any );

		// make sure that 'member' belongs to this type
		co::ICompositeType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.IMethod" );

		return res;
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createIMethodReflector()
{
    return new IMethod_Reflector;
}

} // namespace co
