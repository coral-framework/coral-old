/*******************************************************************************
** Reflection code generated for type 'co.CSLError'
**
** Created: Fri Jul 02 00:14:15 2010
**      by: Coral Compiler version 0.1.0
**
** WARNING! All changes made in this file will be lost when recompiling!
********************************************************************************/

#include <co/CSLError.h>
#include <co/reserved/ReflectorBase.h>
#include <co/MethodInfo.h>
#include <co/AttributeInfo.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
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
	co::CSLError* checkInstance( const co::Any& instance, co::MemberInfo* member )
	{
		if( !member )
			throw co::IllegalArgumentException( "illegal null member info" );

		co::StructType* myType = co::typeOf<co::CSLError>::get();

		// make sure that 'instance' is an instance of this type
		if( instance.getKind() != co::TK_STRUCT ||
			instance.getType() != myType ||
			instance.getState().data.ptr == NULL )
			CORAL_THROW( co::IllegalArgumentException, "expected a valid co::CSLError*, but got " << instance );

		// make sure that 'member' belongs to this type
		co::CompoundType* owner = member->getOwner();
		if( owner != myType )
			CORAL_THROW( co::IllegalArgumentException, "member '" << member->getName() << "' belongs to "
				<< owner->getFullName() << ", not to " << myType->getFullName() );

		return reinterpret_cast<co::CSLError*>( instance.getState().data.ptr );
	}
};

// ------ Reflector Creation Function ------ //

co::Reflector* __createCSLErrorReflector()
{
    return new CSLError_Reflector;
}

} // namespace co
