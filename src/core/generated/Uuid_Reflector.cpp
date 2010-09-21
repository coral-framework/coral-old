/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <Uuid_Adapter.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/IllegalCastException.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class Uuid_Reflector : public co::ReflectorBase
{
public:
	Uuid_Reflector()
	{
		// empty
	}

	virtual ~Uuid_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::Uuid>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::Uuid);
	}

	void createValue( void* address, size_t length )
	{
		checkValidSize( sizeof(co::Uuid), length );
		new( address ) co::Uuid;
    }

	void copyValue( const void* fromAddress, void* toAddress )
	{
		*reinterpret_cast<co::Uuid*>( toAddress ) = *reinterpret_cast<const co::Uuid*>( fromAddress );
    }

	void destroyValue( void* address )
	{
		callDestructor( reinterpret_cast<co::Uuid*>( address ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::Uuid& r = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< bool >( co::Uuid_Adapter::getIsNull( r ) ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::Uuid& r = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		raiseAttributeIsReadOnly( ai ); break;
		default:	raiseUnexpectedMemberIndex();
		}
		CORAL_UNUSED( r );
		CORAL_UNUSED( value );
	}

	void invokeMethod( const co::Any& instance, co::MethodInfo* mi, co::ArrayRange<co::Any const> args, co::Any& res )
	{
		co::Uuid& r = checkInstance( instance, mi );
		checkNumArguments( mi, args.getSize() );
		int argIndex = -1;
		try
		{
			switch( mi->getIndex() )
			{
			case 1:
				{
					co::Uuid_Adapter::clear( r );
				}
				break;
			case 2:
				{
					co::Uuid_Adapter::createRandom( r );
				}
				break;
			case 3:
				{
					const std::string& data_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					co::Uuid_Adapter::createSha1( r, data_ );
				}
				break;
			case 4:
				{
					std::string& str_ = args[++argIndex].get< std::string& >();
					argIndex = -1;
					co::Uuid_Adapter::getString( r, str_ );
				}
				break;
			case 5:
				{
					const std::string& str_ = args[++argIndex].get< const std::string& >();
					argIndex = -1;
					co::Uuid_Adapter::setString( r, str_ );
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
			raiseArgumentTypeException( mi, argIndex, e );
		}
		catch( ... )
		{
			throw;
		}
		CORAL_UNUSED( res );
	}

private:
	co::Uuid& checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::NativeClassType* myType = co::typeOf<co::Uuid>::get();

		if( any.getKind() != co::TK_NATIVECLASS || any.getType() != myType || any.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::Uuid*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.Uuid" );

		return *reinterpret_cast<co::Uuid*>( any.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createUuidReflector()
{
    return new Uuid_Reflector;
}

} // namespace co
