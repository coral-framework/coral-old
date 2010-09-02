/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ANYVALUE_H_
#define _CO_ANYVALUE_H_

#include <co/Any.h>

namespace co {

// Forward Declarations:
class Reflector;

/*!
	Auxiliary class that provides a temporary instance of an arbitrary Coral value.
	Useful in reflective code, such as scripting language integrations.
 
	Except for primitive values, a co::Any by itself can only store \e references to Coral objects.
	However, it can be paired with a co::AnyValue to create a temporary instance of a Coral value
	&mdash; e.g., for use as an output parameter of a dynamically-invoked method. Support for this
	specific use case is available through the makeOut() and makeIn() methods.
 */
class CORAL_EXPORT AnyValue
{
public:
	//! Union of primitive values.
	typedef __any::State::Data PrimitiveData;

	//! A 'neutral' std::vector type.
	typedef std::vector<uint8> PseudoVector;

public:
	//! Creates an invalid co::AnyValue.
	inline AnyValue() : _kind( TK_NONE )
	{;}

	//! Destructor: simply calls clear().
	inline ~AnyValue()
	{
		clear();
	}

	/*!
		Calls the appropriate destructors, releases any allocated memory
		and invalidates the co::AnyValue.
	 */
	void clear();

	//! \name Managing Primitive Values
	//@{

	inline PrimitiveData& createPrimitive( TypeKind kind )
	{
		if( _kind != TK_NONE ) clear();
		_kind = kind;
		return getPrimitive();
	}

	inline PrimitiveData& getPrimitive()
	{
		assert( _kind > TK_ANY && ( _kind < TK_STRING || _kind == TK_ENUM || _kind == TK_INTERFACE ) );
		return _v.data;
	}

	//@}
	//! \name Managing co::Any Values
	//@{

	//! Creates an instance of co::Any.
	inline Any& createAny()
	{
		if( _kind != TK_NONE ) clear();
		new( _v.anyArea ) Any();
		_kind = TK_ANY;
		return *reinterpret_cast<Any*>( _v.anyArea );
	}

	inline co::Any& getAny()
	{
		assert( _kind == TK_ANY );
		return *reinterpret_cast<Any*>( _v.anyArea );
	}

	//@}
	//! \name Managing String Values
	//@{

	//! Creates an instance of std::string.
	inline std::string& createString()
	{
		if( _kind != TK_NONE ) clear();
		new( _v.stringArea ) std::string();
		_kind = TK_STRING;
		return *reinterpret_cast<std::string*>( _v.stringArea );
	}

	inline std::string& getString()
	{
		assert( _kind == TK_STRING );
		return *reinterpret_cast<std::string*>( _v.stringArea );
	}

	//@}
	//! \name Managing Arrays
	//@{

	/*!
		Creates a std::vector (or a std::RefVector, if \c elementType is an interface)
		with \c n default-constructed elements of type \c elementType.
	 */
	PseudoVector& createArray( Type* elementType, size_t n = 0 );

	inline PseudoVector& getArray()
	{
		assert( _kind == TK_ARRAY );
		return *reinterpret_cast<PseudoVector*>( _v.array.vectorArea );
	}

	//@}
	//! \name Managing Complex Values (structs and native classes)
	//@{

	/*!
		Creates a default-constructed instance of \c type.
		Note that \c type must be a struct or native class type.
	 */
	void* createComplexValue( Type* type );

	inline void* getComplexValue()
	{
		assert( _kind == TK_STRUCT || _kind == TK_NATIVECLASS );
		return ( _kind == TK_STRUCT ? _v.complex.ptr : _v.complex.inplaceArea );
	}

	//@}
	//! \name Using a co::AnyValue as an Output Argument
	//@{

	/*!
		Prepares a {co::Any,co::AnyValue} pair for use as an 'out' argument of the given type.
		\param paramType the 'out' parameter type.
		\param arg the co::Any that will be passed to the method; it may contain
					a value (that will be preserved) if the parameter is 'inout'.
		\throw co::Exception if 'arg' contains a value incompatible with 'paramType', etc.
	 */
	void makeOut( Type* paramType, Any& arg );

	/*!
		Converts a {co::Any,Variant} pair that's currently an 'out' argument into an 'in' argument.
		\param arg the co::Any that will be passed to the method.
		\note This method never raises exceptions.
	 */
	void makeIn( Any& arg );

	//@}

private:
	/*
		Performance Setting: number of 'doubles' that can be stored in a AnyValue
		without resorting to dynamic memory allocation. For instance, if you want
		your double-precision Quaternion native class to be handled efficiently,
		set this to 4. If you want your single-precision (float) 4x4 matrix class
		to be handled efficiently, you could set this to 8. Please note that large
		values lead to excessive memory consumption and performance degradation.
		Also note that it does not make sense to set this value to less than 2.
	 */
	static const int INPLACE_DOUBLES = 4;
	static const size_t INPLACE_CAPACITY = sizeof(double) * INPLACE_DOUBLES;

	union
	{
		PrimitiveData data;
		uint8 anyArea[sizeof(Any)];
		uint8 stringArea[sizeof(std::string)];
		struct
		{
			Reflector* reflector;
			uint8 vectorArea[sizeof(PseudoVector)];
		}
		array;
		struct
		{
			Reflector* reflector;
			union { void* ptr; double inplaceArea[INPLACE_DOUBLES]; };
		}
		complex;
	}
	_v;

	/*
		For complex values, 'kind' has special meanings.
			- co::TK_STRUCT: the CV is larger than INPLACE_CAPACITY and is heap-allocated.
			- co::TK_NATIVECLASS: the CV fits in INPLACE_CAPACITY and is kept in place.
	 */
	TypeKind _kind;
};

} // namespace co

#endif // _CO_ANYVALUE_H_
