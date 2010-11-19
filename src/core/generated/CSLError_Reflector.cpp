/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/CSLError.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Reflector ------ //

class CSLError_Reflector : public co::ReflectorBase
{
public:
	CSLError_Reflector()
	{
		// empty
	}

	virtual ~CSLError_Reflector()
	{
		// empty
	}

	co::Type* getType()
	{
		return co::typeOf<co::CSLError>::get();
	}

	co::int32 getSize()
	{
		return sizeof(co::CSLError);
	}

	void createValue( void* address, size_t length )
	{
		checkValidSize( sizeof(co::CSLError), length );
		new( address ) co::CSLError;
    }

	void copyValue( const void* fromAddress, void* toAddress )
	{
		*reinterpret_cast<co::CSLError*>( toAddress ) = *reinterpret_cast<const co::CSLError*>( fromAddress );
    }

	void destroyValue( void* address )
	{
		callDestructor( reinterpret_cast<co::CSLError*>( address ) );
	}

	void getAttribute( const co::Any& instance, co::AttributeInfo* ai, co::Any& value )
	{
		co::CSLError* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->filename ); break;
		case 1:		value.set< co::int32 >( p->line ); break;
		case 2:		value.set< const std::string& >( p->message ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setAttribute( const co::Any& instance, co::AttributeInfo* ai, const co::Any& value )
	{
		co::CSLError* p = checkInstance( instance, ai );
		switch( ai->getIndex() )
		{
		case 0:		p->filename = value.get< const std::string& >(); break;
		case 1:		p->line = value.get< co::int32 >(); break;
		case 2:		p->message = value.get< const std::string& >(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

private:
	co::CSLError* checkInstance( const co::Any& any, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		// make sure that 'any' is an instance of this type
		co::StructType* myType = co::typeOf<co::CSLError>::get();

		if( any.getKind() != co::TK_STRUCT || any.getType() != myType || any.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::CSLError*, but got " << any );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to co.CSLError" );

		return reinterpret_cast<co::CSLError*>( any.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createCSLErrorReflector()
{
    return new CSLError_Reflector;
}

} // namespace co
