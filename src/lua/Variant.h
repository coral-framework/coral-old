/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _VARIANT_H_
#define _VARIANT_H_

#include <co/Any.h>
#include <co/Reflector.h>
#include <cstdlib>

/*!
	Auxiliary struct for reading values from Lua.
	For complex values, 'kind' has special meanings.
		- co::TK_STRUCT: the CV is > MAX_INPLACE_SIZE bytes and is heap-allocated.
		- co::TK_NATIVECLASS: the CV is <= MAX_INPLACE_SIZE bytes and is kept in place.
 */
struct Variant
{
	static const int NUM_INPLACE_DBLS = 4;
	static const size_t MAX_INPLACE_SIZE = sizeof(double) * NUM_INPLACE_DBLS;

	union
	{
		co::__any::State::Data data;
		co::uint8 __anyArea[sizeof(co::Any)];
		co::uint8 __stringArea[sizeof(std::string)];
		struct
		{
			co::Reflector* reflector;
			union { void* ptr; double v[NUM_INPLACE_DBLS]; };
		}
		complexValue;
	};

	co::TypeKind kind;

	inline Variant() : kind( co::TK_NONE )
	{
		data.ptr = NULL;
	}

	inline ~Variant()
	{
		clear();
	}

	inline void clear()
	{
		switch( kind )
		{
		case co::TK_ANY:
			reinterpret_cast<co::Any*>( __anyArea )->~Any();
			break;
		case co::TK_STRING:
			reinterpret_cast<std::string*>( __stringArea )->~basic_string();
			break;
		case co::TK_STRUCT:
			complexValue.reflector->destroyValue( complexValue.ptr );
			free( complexValue.ptr );
			break;
		case co::TK_NATIVECLASS:
			complexValue.reflector->destroyValue( complexValue.v );
			break;
		default:
			break;
		}
	}
	
	inline void createAny()
	{
		clear();
		new( __anyArea ) co::Any();
		kind = co::TK_ANY;
	}

	inline void createString()
	{
		clear();
		new( __stringArea ) std::string();
		kind = co::TK_STRING;
	}

	inline void* createComplexValue( co::Type* type )
	{
		clear();
		complexValue.reflector = type->getReflector();
		size_t size = complexValue.reflector->getSize();
		void* ptr;
		if( size > MAX_INPLACE_SIZE )
		{
			kind = co::TK_STRUCT;
			ptr = complexValue.ptr = malloc( size );
		}
		else
		{
			kind = co::TK_NATIVECLASS;
			ptr = complexValue.v;
		}
		complexValue.reflector->createValue( ptr, size );
		return ptr;
	}

	inline co::Any& getAny()
	{
		assert( kind == co::TK_ANY );
		return *reinterpret_cast<co::Any*>( __anyArea );
	}

	inline std::string& getString()
	{
		assert( kind == co::TK_STRING );
		return *reinterpret_cast<std::string*>( __stringArea );
	}

	inline void* getComplexValue()
	{
		assert( kind == co::TK_STRUCT || kind == co::TK_NATIVECLASS );
		return ( kind == co::TK_STRUCT ? complexValue.ptr : complexValue.v );
	}

	/*!
		Prepares a {co::Any,Variant} pair for use as an 'out' argument of the given type.
		\param type the 'out' parameter type.
		\param arg the co::Any that will be passed to the method; it may contain
					a value (that must be preserved) if the parameter is 'inout'.
		This method is allowed to raise exceptions.
	 */
	void makeOut( co::Type* type, co::Any& arg );

	/*!
		Converts a {co::Any,Variant} pair that's currently an 'out' argument into an 'in' argument.
		\param arg the co::Any that will be passed to the method.
		This method is NOT allowed to raise exceptions.
	 */
	void makeIn( co::Any& arg );
};

#endif
