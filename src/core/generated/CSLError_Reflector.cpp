/*
 * Coral - Lightweight C++ Component Framework
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

	void createValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			new( reinterpret_cast<co::CSLError*>( ptr ) + i ) co::CSLError;
    }

	void copyValues( const void* fromPtr, void* toPtr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			reinterpret_cast<co::CSLError*>( toPtr )[i] = reinterpret_cast<const co::CSLError*>( fromPtr )[i];
    }

	void destroyValues( void* ptr, size_t numValues )
	{
		for( size_t i = 0; i < numValues; ++i )
			callDestructor( reinterpret_cast<co::CSLError*>( ptr ) + i );
	}

	void getField( co::Any instance, co::IField* field, co::AnyValue& value )
	{
		co::CSLError* p = co::checkInstance<co::CSLError>( instance, field );
		switch( field->getIndex() )
		{
		case 0:		value = p->filename; break;
		case 1:		value = p->line; break;
		case 2:		value = p->message; break;
		default:	raiseUnexpectedMemberIndex();
		}
	}

	void setField( co::Any instance, co::IField* field, co::Any value )
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
