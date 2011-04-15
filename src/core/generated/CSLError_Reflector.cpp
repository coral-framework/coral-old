/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/CSLError.h>
#include <co/IField.h>
#include <co/MissingInputException.h>
#include <co/IllegalArgumentException.h>
#include <co/reserved/ReflectorBase.h>
#include <sstream>
#include <cassert>

namespace co {

// ------ Reflector Component ------ //

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

	co::IType* getType()
	{
		return co::typeOf<co::CSLError>::get();
	}

	co::uint32 getSize()
	{
		return sizeof(co::CSLError);
	}

	void createValue( void* address )
	{
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

	void getField( const co::Any& instance, co::IField* field, co::Any& value )
	{
		co::CSLError* p = co::checkInstance<co::CSLError>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value.set< const std::string& >( p->filename ); break;
		case 1:		value.set< co::int32 >( p->line ); break;
		case 2:		value.set< const std::string& >( p->message ); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( const co::Any& instance, co::IField* field, const co::Any& value )
	{
		co::CSLError* p = co::checkInstance<co::CSLError>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		p->filename = value.get< const std::string& >(); break;
		case 1:		p->line = value.get< co::int32 >(); break;
		case 2:		p->message = value.get< const std::string& >(); break;
		default:	raiseUnexpectedMemberIndex();
		}
	}
};

// ------ Reflector Creation Function ------ //

co::IReflector* __createCSLErrorReflector()
{
    return new CSLError_Reflector;
}

} // namespace co
