/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_ANY_H_
#define _CO_ANY_H_

#include <co/Coral.h>
#include <co/IService.h>
#include <co/RefVector.h>
#include <co/TypeTraits.h>

#include <co/IEnum.h>
#include <co/IArray.h>
#include <co/IStruct.h>
#include <co/IException.h>
#include <co/IInterface.h>
#include <co/IComponent.h>
#include <co/INativeClass.h>

namespace co {

#ifndef DOXYGEN

//! Private namespace with auxiliary code for class co::Any.
namespace __any {

struct State
{
	union Data
	{
		// Storage for primitive values:
		bool b;
		int8 i8;
		uint8 u8;
		int16 i16;
		uint16 u16;
		int32 i32;
		uint32 u32;
		int64 i64;
		uint64 u64;
		float f;
		double d;

		// Multipurpose pointers:
		void* ptr;
		const void* cptr;
		IService* service;
	}
	data;

	// co::IType of the variable; or, if this is an array, the array's element type.
	IType* type;

	// convenience method to get 'type' downcasted to an IInterface
	inline IInterface* getInterface() const { return static_cast<IInterface*>( type ); }

	// only used if arrayKind is AK_Range
	uint32 arraySize;

	// the kind of variable we're holding
	uint8 kind;

	// facts about the variable type (or, if this is an array, the array element type):
	uint8 isConst : 1;
	uint8 isPointer : 1;
	uint8 isPointerConst : 1;
	uint8 isReference : 1;

	// what array representation was provided
	enum ArrayKind { AK_StdVector, AK_RefVector, AK_Range };
	uint8 arrayKind;

	// reserved for use in co::Any:
	uint8 objectKind;
};

template<typename TT>
struct PrepareBasic
{
	inline static void prepare( State& s )
	{
		s.kind = TT::kind;
		s.isConst = TT::isConst;
		s.isPointer = TT::isPointer;
		s.isPointerConst = TT::isPointerConst;
		s.isReference = TT::isReference;
	}
};

template<typename TT>
struct PrepareWithType
{
	inline static void prepare( State& s )
	{
		PrepareBasic<TT>::prepare( s );
		s.type = typeOf<typename TT::CoreType>::get();
	}
};

template<TypeKind kind, typename TT>
struct PrepareStateFor
{
	inline static void prepare( State& s )
	{
		PrepareBasic<TT>::prepare( s );
		s.type = NULL;
	}
};

template<typename TT>
struct PrepareStateFor<TK_ENUM, TT> : public PrepareWithType<TT> {};

template<typename TT>
struct PrepareStateFor<TK_STRUCT, TT> : public PrepareWithType<TT> {};

template<typename TT>
struct PrepareStateFor<TK_NATIVECLASS, TT> : public PrepareWithType<TT> {};

template<typename TT>
struct PrepareStateFor<TK_INTERFACE, TT> : public PrepareWithType<TT> {};

template<typename T, typename ATT>
struct PrepareStateForArray
{
	static_assert( sizeof(T)<0, "unsupported array representation" );
};

template<typename T, typename ATT>
struct PrepareStateForArray<std::vector<T>, ATT>
{
	typedef traits::get<T> ETT; // traits for the array element type
	inline static void prepare( State& s )
	{
		static_assert( ETT::kind != TK_ARRAY, "arrays of arrays are not supported" );
		static_assert( ATT::isReference && !ATT::isPointer, "std::vectors must be passed by reference" );
		s.type = typeOf<typename ETT::CoreType>::get();
		s.isConst = ETT::isConst;
		s.isPointer = ETT::isPointer;
		s.isPointerConst = ETT::isPointerConst;
		s.isReference = ETT::isReference;
		s.arrayKind = State::AK_StdVector;
	}
};

template<typename T, typename ATT>
struct PrepareStateForArray<RefVector<T>, ATT>
{
	inline static void prepare( State& s )
	{
		static_assert( ATT::isReference && !ATT::isPointer, "co::RefVectors must be passed by reference" );
		s.type = typeOf<T>::get();
		s.isConst = false;
		s.isPointer = true;
		s.isPointerConst = true;
		s.isReference = false;
		s.arrayKind = State::AK_RefVector;
	}
};

template<typename T, typename ATT>
struct PrepareStateForArray<Range<T>, ATT>
{
	typedef traits::get<T> ETT; // traits for the array element type
	inline static void prepare( State& s )
	{
		static_assert( ETT::kind != TK_ARRAY, "arrays of arrays are not supported" );
		static_assert( !ATT::isPointer && !ATT::isReference, "co::Ranges must be passed by value" );
		s.type = typeOf<typename ETT::CoreType>::get();
		s.isConst = ETT::isConst;
		s.isPointer = ETT::isPointer;
		s.isPointerConst = ETT::isPointerConst;
		s.isReference = ETT::isReference;
		s.arrayKind = State::AK_Range;
	}
};

template<typename ATT>
struct PrepareStateFor<TK_ARRAY, ATT>
{
	inline static void prepare( State& s )
	{
		PrepareStateForArray<typename ATT::CoreType, ATT>::prepare( s );
		s.kind = TK_ARRAY;
	}
};

template<TypeKind kind, typename TT, typename T>
struct PrepareStateForStorageOf
{
	inline static void prepare( State& s, T )
	{
		PrepareStateFor<kind, TT>::prepare( s );
	}
};

template<typename TT, typename T>
struct PrepareStateForStorageOf<TK_INTERFACE, TT, T*>
{
	inline static void prepare( State& s, T* v )
	{
		PrepareBasic<TT>::prepare( s );
		s.type = v ? v->getInterface() : typeOf<T>::get();
	}
};

template<typename TT, typename T>
struct PrepareStateForStorageOf<TK_INTERFACE, TT, RefPtr<T>&>
{
	inline static void prepare( State& s, RefPtr<T>& v )
	{
		PrepareBasic<TT>::prepare( s );
		s.type = v.isValid() ? v->getInterface() : typeOf<T>::get();
	}
};

template<TypeKind kind, typename T>
struct ValueHelper
{
	// this is the final destination of all unrecognized values
	static void store( State&, T ) { static_assert( sizeof(T)<0, "variable must be passed by reference" ); }
	static T retrieve( State& ) { static_assert( sizeof(T)<0, "variable must be retrieved by reference" ); }
};

template<typename T>
struct ValueHelper<TK_ARRAY, T>
{
	// std::vectors and co::RefVectors are handled by this case
	static void store( State&, T ) { static_assert( sizeof(T)<0, "array type must be passed by reference" ); }
	static T retrieve( State& ) { static_assert( sizeof(T)<0, "array type must be retrieved by reference" ); }
};

template<typename T>
struct ValueHelper<TK_ARRAY, Range<T> >
{
	// co::Ranges are handled by this case
	inline static void store( State& s, Range<T>& v )
	{
		s.data.cptr = &( v.getFirst() );
		size_t size = v.getSize();
		assert( size < MAX_UINT32 );
		s.arraySize = static_cast<uint32>( size );
	}

	inline static Range<T> retrieve( State& s )
	{
		if( s.arrayKind == State::AK_Range )
			return Range<T>( reinterpret_cast<T*>( s.data.ptr ), s.arraySize );
		else
			return Range<T>( *reinterpret_cast<std::vector<typename
										traits::removeConst<T>::Type>*>( s.data.ptr ) );
	}
};

template<typename T>
struct ValueHelper<TK_ENUM, T>
{
	// enums are handled by this case
	inline static void store( State& s, T v ) { s.data.u32 = static_cast<uint32>( v ); }
	inline static T retrieve( State& s ) { return static_cast<T>( s.data.u32 ); }
};

template<typename T>
struct VariableHelper
{
	// forward all unrecognized variables to ValueHelper
	typedef traits::get<T> TT;
	inline static void store( State& s, T v ) { ValueHelper<TT::kind, T>::store( s, v ); }
	inline static T retrieve( State& s ) { return ValueHelper<TT::kind, T>::retrieve( s ); }
};

template<typename T>
struct VariableHelper<const T>
{
	inline static void store( State& s, const T v ) { VariableHelper<T>::store( s, v ); }
	inline static const T retrieve( State& s ) { return VariableHelper<T>::retrieve( s ); }
};

template<typename T>
struct VariableHelper<const T*>
{
	inline static void store( State& s, const T* v ) { VariableHelper<T*>::store( s, const_cast<T*>( v ) ); }
	inline static const T* retrieve( State& s ) { return VariableHelper<T*>::retrieve( s ); }
};

template<typename T>
struct VariableHelper<T* const>
{
	inline static void store( State& s, T* const ptr ) { VariableHelper<T*>::store( s, const_cast<T*>( ptr ) ); }
	inline static T* const retrieve( State& s ) { return VariableHelper<T*>::retrieve( s ); }
};

template<typename T>
struct VariableHelper<const T* const>
{
	inline static void store( State& s, const T* const ptr ) { VariableHelper<T*>::store( s, const_cast<T*>( ptr ) ); }
	inline static const T* const retrieve( State& s ) { return VariableHelper<T*>::retrieve( s ); }
};

template<typename T>
struct VariableHelper<const T&>
{
	inline static void store( State& s, const T& v ) { VariableHelper<T&>::store( s, const_cast<T&>( v ) ); }
	inline static const T& retrieve( State& s ) { return VariableHelper<T&>::retrieve( s ); }
};

template<TypeKind kind, typename T>
struct PointerHelper
{
	// handles general pointers & references
	inline static void store( State& s, T* ptr ) { s.data.ptr = ptr; }
	inline static T* retrieve( State& s ) { return reinterpret_cast<T*>( s.data.ptr ); }
};

template<typename T>
struct PointerHelper<TK_INTERFACE, T>
{
	// handles services
	inline static void store( State& s, T* ptr ) { s.data.service = ptr; }
	inline static T* retrieve( State& s ) { return static_cast<T*>( s.data.service ); }
};

template<typename T>
struct VariableHelper<T*>
{
	// all pointers are forwarded to PointerHelper
	typedef traits::get<T> TT;
	inline static void store( State& s, T* ptr ) { PointerHelper<TT::kind, T>::store( s, ptr ); }
	inline static T* retrieve( State& s ) { return PointerHelper<TT::kind, T>::retrieve( s ); }
};

template<typename T>
struct VariableHelper<T&>
{
	inline static void store( State& s, T& v ) { PointerHelper<TK_NONE, T>::store( s, &v ); }
	inline static T& retrieve( State& s ) { return *PointerHelper<TK_NONE, T>::retrieve( s ); }
};

template<>
struct VariableHelper<bool>
{
	inline static void store( State& s, bool v ) { s.data.b = v; }
	inline static bool retrieve( State& s ) { return s.data.b; }
};

template<>
struct VariableHelper<int8>
{
	inline static void store( State& s, int8 v ) { s.data.i8 = v; }
	inline static int8 retrieve( State& s ) { return static_cast<int8>( s.data.i8 ); }
};

template<>
struct VariableHelper<uint8>
{
	inline static void store( State& s, uint8 v ) { s.data.u8 = v; }
	inline static uint8 retrieve( State& s ) { return static_cast<uint8>( s.data.u8 ); }
};

template<>
struct VariableHelper<int16>
{
	inline static void store( State& s, int16 v ) { s.data.i16 = v; }
	inline static int16 retrieve( State& s ) { return static_cast<int16>( s.data.i16 ); }
};

template<>
struct VariableHelper<uint16>
{
	inline static void store( State& s, uint16 v ) { s.data.u16 = v; }
	inline static uint16 retrieve( State& s ) { return static_cast<uint16>( s.data.u16 ); }
};

template<>
struct VariableHelper<int32>
{
	inline static void store( State& s, int32 v ) { s.data.i32 = v; }
	inline static int32 retrieve( State& s ) { return s.data.i32; }
};

template<>
struct VariableHelper<uint32>
{
	inline static void store( State& s, uint32 v ) { s.data.u32 = v; }
	inline static uint32 retrieve( State& s ) { return s.data.u32; }
};

template<>
struct VariableHelper<int64>
{
	inline static void store( State& s, int64 v ) { s.data.i64 = v; }
	inline static int64 retrieve( State& s ) { return s.data.i64; }
};

template<>
struct VariableHelper<uint64>
{
	inline static void store( State& s, uint64 v ) { s.data.u64 = v; }
	inline static uint64 retrieve( State& s ) { return s.data.u64; }
};

template<>
struct VariableHelper<float>
{
	inline static void store( State& s, float v ) { s.data.f = v; }
	inline static float retrieve( State& s ) { return s.data.f; }
};

template<>
struct VariableHelper<double>
{
	inline static void store( State& s, double v ) { s.data.d = v; }
	inline static double retrieve( State& s ) { return s.data.d; }
};

} // namespace __any

#endif // DOXYGEN

/*!
	\brief An intermediate representation for variables in the Coral type system, used for reflection.

	By design, the state of a co::Any is \e transient, so its instances <b>should only be allocated
	on the stack</b>. This class is optimized for use in C++ method signatures (as a parameter or
	return type) and is <b>unsafe for use as a field type or member variable</b>.

	\par Supported Features:
		- Type checks, including compliance with type const'ness.
		- Automatic conversion between primitive values (boolean, arithmetic types and enum).
		- Type-safe storage and retrieval of pointers and references.
		- Storage and retrieval of arrays as pointers to \c std::vectors, \c co::RefVectors or \c co::Ranges.

	\par What is NOT supported:
		- Storing exceptions.
		- Reference counting of services. Stored service pointers are \b not considered active references.

	\par Values vs. References
		For efficiency, this class only allows the storage of values for \c enums and <em>primitive types</em>
		(i.e. \c bool, <tt>[u]int{8|16|32|64}</tt>, \c float and \c double). All other types must be stored and
		retrieved <em>by reference</em> (i.e. either as pointers or actual references), and the programmer must
		guarantee referred objects outlive their co::Any instances.
		\par
		For variables stored by reference, it is possible to retrieve a pointer even if the passed variable was
		a reference. However, the opposite (retrieving a reference where a pointer was passed) is \b not
		allowed, because the pointer could be NULL. This is not to be confused with retrieving a reference to a
		pointer (<tt>Foo*&</tt>), which is allowed if the passed variable was truly a reference to a pointer.

	\par Arrays
		An array can be passed using three possible representations:
		  -# A \c co::Range, which is the most generic representation but cannot be used to add/remove elements;
		  -# A \c std::vector, which is useful when the receiver needs to add/remove elements to/from the array;
		  -# Or a \c co::RefVector, which is similar to a \c std::vector, but keeps active references to interfaces.
		\par
		These types are \e always considered to be \b mutable. In other words, all arrays passed as \c std::vectors
		or \c co::RefVectors can have their contents changed. To prevent the addition/removal of elements, one should
		pass the array as a \c co::Range instead. Moreover, to prevent existing elements from being modified,
		one should pass a \c co::Range of \c <b>const</b> elements.
		\par
		For instance, an array passed as a <tt>co::Range<const std::string></tt> is completely immutable &mdash;
		as opposed to a <tt>std::vector<std::string></tt>, which is fully mutable. Somewhere in between, a
		<tt>co::Range<std::string></tt> allows existing strings to be modified, but not the array length.
		\par
		Arrays represented by \c std::vectors or \c co::RefVectors must always be passed and retrieved
		<b>by reference</b>, while \c co::Ranges must always be passed and retrieved <b>by value</b>.
		\par
		Arrays must generally be retrieved by the exact same type they were passed. However, when
		retrieving \c co::Ranges the following coercion rules apply:
		  - It is possible to retrieve a <tt>co::Range<\e ValueType></tt> from an array passed as a
				<tt>std::vector<\e ValueType></tt>.
		  - It is possible to retrieve a <tt>co::Range<const \e ValueType></tt> from an array passed as
				either a <tt>co::Range<\e ValueType></tt> or a <tt>std::vector<\e ValueType></tt>.
		  - It is possible to retrieve a <tt>co::Range<\e SuperType* const></tt> from an array
				passed as either a <tt>co::Range<\e SubType* [const]></tt>, a
				<tt>std::vector<\e SubType*></tt> or a <tt>co::RefVector<\e SubType></tt>.
		\par
		Non-listed cases must be retrieved by exact type.
		In all rules, \e ValueType is a Coral value (such as a \c bool or a \c struct), optionally
		passed by reference; and both \e SubType and \e SuperType are Coral interfaces, where \e SubType
		is a subtype of \e SuperType, or both are the same interface.

 	\par Automatic Conversions for Values
		All types storable as values in a co::Any can be converted to any other value type.
		This includes \c enums and all primitive types (such as \c bool, integers and floats),
		and excludes types passed by reference (most notably strings).
		\par
		Conversion rules are the same applied for \c static_casts by your C++ compiler.
		Enums are seen as \c co::uint32 values. However, when converting from a different type, a check is made
		to ensure the result maps to a valid enum identifier (otherwise, a co::IllegalCastException is raised).
 */
class CORAL_EXPORT Any
{
public:
	//! Alias to the struct that contains all info about a co::Any's var.
	typedef __any::State State;

	//! A 'neutral' std::vector type.
	typedef std::vector<uint8> PseudoVector;

	/*!
		Flags that can be OR'ed together to describe variables in the Coral type system.
		For use with the custom variable setters and constructors.
	 */
	enum VariableFlags
	{
		VarIsValue			= 0,		//!< Indicates the variable is a simple value.
		VarIsConst			= 1 << 0,	//!< Indicates the referred value is 'const'.
		VarIsPointer		= 1 << 1,	//!< Indicates the variable is a pointer.
		VarIsPointerConst	= 1 << 2,	//!< Indicates the pointer is 'const' (implies VarIsPointer).
		VarIsReference		= 1 << 3,	//!< Indicates the variable is a reference.
	};

	/*!
		Enumeration of the supported array representations in the Coral type system.
		For use with setArray() and its corresponding co::Any constructor.
	 */
	enum ArrayKind
	{
		AK_StdVector,	//!< Indicates the variable is a \c std::vector.
		AK_RefVector,	//!< Indicates the variable is a \c co::RefVector (implies VarIsPointerConst).
		AK_Range	//!< Indicates the variable is a \c co::Range.
	};

	/*!
		Performance Settings:

		MIN_CAPACITY: minimum number of bytes that should be storable in a co::Any
		without resorting to dynamic memory allocation. For instance, if you want
		your double-precision quaternion native class to be handled efficiently, set
		this to 32. If you want your single-precision (float) 4x4 matrix class to
		be handled efficiently, you could set this to 64. Please note that large
		values lead to excessive memory consumption and performance degradation.
		Settings this to less than 3*sizeof(void*) makes no sense on most platforms.

		INPLACE_CAPACITY: actual number of bytes that can be stored in a co::Any
		without resorting to dynamic memory allocation. Automatically set based
		on MIN_CAPACITY and the size of other basic types.
	 */
	enum PerformanceSettings
	{
		MIN_CAPACITY = 4 * sizeof(double),
		REQ_VEC_CAPACITY = sizeof(PseudoVector),
		REQ_STR_CAPACITY = sizeof(std::string),
		REQ_BASIC_CAPACITY = REQ_VEC_CAPACITY > REQ_STR_CAPACITY ? REQ_VEC_CAPACITY : REQ_STR_CAPACITY,
		INPLACE_CAPACITY = MIN_CAPACITY > REQ_BASIC_CAPACITY ? MIN_CAPACITY : REQ_BASIC_CAPACITY
	};

public:
	//! Creates an invalid co::Any.
	inline Any() : _state()
	{;}

	/*!
		\brief Template constructor that stores any variable supported by the Coral type system.

		\warning Since the template variable \a T must be inferred by the compiler, this constructor
			is subject to language limitations that will make it miss the fact that a variable's type
			is 'const' and/or a reference. Please call set() instead, passing the variable's type explicitly,
			if you must store a reference or a 'const' variable.
	 */
	template<typename T>
	inline Any( T var ) :  _state()
	{
		set<T>( var );
	}

	/*!
		\brief Constructor corresponding to a setService() call.
		Please, see setService()'s documentation for more info.
	 */
	inline Any( IService* instance, IInterface* type ) : _state()
	{
		setService( instance, type );
	}

	/*!
		\brief Constructor corresponding to a setVariable() call.
		Please, see setVariable()'s documentation for more info.
	 */
	inline Any( IType* type, uint32 flags, void* ptr ) : _state()
	{
		setVariable( type, flags, ptr );
	}

	/*!
		\brief Constructor corresponding to a setBasic() call.
		Please, see setBasic()'s documentation for more info.
	 */
	inline Any( TypeKind kind, uint32 flags, void* ptr ) : _state()
	{
		setBasic( kind, flags, ptr );
	}

	/*!
		\brief Constructor corresponding to a setArray() call.
		Please, see setArray()'s documentation for more info.
	 */
	inline Any( ArrayKind arrayKind, IType* elementType, uint32 flags, void* ptr, size_t arraySize = 0 )
		: _state()
	{
		setArray( arrayKind, elementType, flags, ptr, arraySize );
	}

	//! Copy constructor.
	inline Any( const Any& other ) : _state()
	{
		copy( other );
	}

	//! Destructor.
	inline ~Any()
	{
		if( _state.objectKind != TK_NONE )
			destroyObject();
	}

	//! Clears any variable/object stored in the co::Any.
	void clear();

	//! \name Variable Introspection
	//@{

	//! Returns whether this co::Any was initialized.
	inline bool isValid() const { return _state.kind != TK_NONE; }

	//! Returns the kind of variable stored in this object.
	inline TypeKind getKind() const { return static_cast<TypeKind>( _state.kind ); }

	//! Returns whether the variable stored in this object is 'const'.
	inline bool isConst() const { return _state.isConst != 0; }

	//! Returns whether the variable stored in this object is a pointer.
	inline bool isPointer() const { return _state.isPointer != 0; }

	//! Returns whether the variable stored in this object is a pointer and the pointer is const.
	inline bool isPointerConst() const { return _state.isPointerConst != 0; }

	//! Returns whether the variable stored in this object is a reference.
	inline bool isReference() const { return _state.isReference != 0; }

	/*!
		For \c enums, \c structs and \c native \c classes, this returns the co::IType of the stored variable.
		For \c arrays, this returns the array element type.
		For \c interfaces you should call getInterface() instead.
		For all other type kinds, this returns NULL.
	 */
	inline IType* getType() const { return _state.type; }

	//! Returns the type of the stored service.
	inline IInterface* getInterface() const
	{
		assert( _state.kind == TK_INTERFACE );
		return static_cast<IInterface*>( _state.type );
	}

	/*!
		Returns the size of the variable stored in this object; or, in the case of arrays,
		by each array element. This method mimics the behavior of operator sizeof(): for pointers,
		the result is always sizeof(void*). For references, the result is equivalent to the value's size.

		This method may have to use the type's reflector to inquire the size of user-defined types;
		therefore, it may throw exceptions.
	 */
	int32 getSize() const;

	/*!
		Attempts to retrieve a stored variable, making the necessary casts whenever possible.

		\throw co::IllegalCastException if there is no valid cast from the stored variable's
				type to the requested type \a T.
	 */
	template<typename T>
	inline T get() const
	{
		typedef traits::get<T> TT;

		static_assert( TT::kind != TK_NONE, "T is not a valid Coral type" );
		static_assert( TT::kind != TK_EXCEPTION, "cannot retrieve exceptions" );

		Any temp;
		__any::PrepareStateFor<TT::kind, TT>::prepare( temp._state );
		temp.castFrom( _state );

		return __any::VariableHelper<T>::retrieve( temp._state );
	}

	//! Provides read/write access to the internal state of a co::Any.
	inline State& getState() { return _state; }

	//! Provides read access to the internal state of a co::Any.
	inline const State& getState() const { return _state; }

	//@}
	//! \name Automatic Variable Storage
	//@{

	/*!
		Automatically stores any variable supported by the Coral type system.
		Previous contents are discarded.

		This method uses template meta-programming to statically infer a variable's type.
		If you need to set a co::Any dynamically at runtime, use the "Custom Storage Methods" instead.

		\warning If you don't specify the template variable \a T explicitly, due to language limitations
			the compiler will generally miss the fact that a variable's type is 'const' and/or a reference.
	 */
	template<typename T>
	inline void set( T var )
	{
		typedef traits::get<T> TT;

		static_assert( TT::kind != TK_NONE, "T is not a valid Coral type" );
		static_assert( TT::kind != TK_EXCEPTION, "cannot store exceptions" );

		__any::PrepareStateForStorageOf<TT::kind, TT, T>::prepare( _state, var );
		__any::VariableHelper<T>::store( _state, var );
	}

	//@}
	/*!
		\name Custom Variable Storage
		These are alternative versions of set() that offer manual control over the passed variables.

		\warning
			These methods are intended for advanced users and should be used with great care.
			They only contain assertions and do not throw exceptions. Incorrect usage may
			cause access violations and memory corruption.
	 */
	//@{

	/*!
		Stores a service.

		The \a type parameter is optional. When not given, it's extracted from the
		\a service, which in this case cannot be NULL. Always provide the \a type
		in places where \a service could be null.

		This method does not take variable flags, variables are always pointers. If you
		want to create a reference to a service pointer, use setVariable() instead.
	 */
	void setService( IService* service, IInterface* type = 0 );

	/*!
		Stores a single-value (non-array) variable.

		\param type The variable's type.
		\param flags One or more flags from the VariableFlags enum OR'ed together.
					Please note that when \c VarIsPointer is specified, \a ptr should
					be a pointer to the value, not a pointer to a pointer.
		\param ptr The variable instance, which must really be an instance of \a type and
					have the modifiers described in \a flags (otherwise, all hell will break
					loose). If \a flags specifies \c VarIsReference, \a ptr cannot be null.
	 */
	void setVariable( IType* type, uint32 flags, void* ptr );

	/*!
		Alternative version of setVariable(), simplified for basic types.
		Parameter 'kind' must range from co::TK_ANY to co::TK_STRING (i.e. types
		that are uniquely identified by their co::TypeKind).
	 */
	void setBasic( TypeKind kind, uint32 flags, void* ptr );

	/*!
		Stores an array of any kind.

		\param arrayKind The array representation being passed, which affects the expected
					value of parameter \a ptr:
					- \c AK_StdVector: \a ptr should be a pointer to a \c std::vector instance.
					- \c AK_RefVector: \a ptr should be a pointer to a \c co::RefVector instance.
					- \c AK_Range: \a ptr should be a pointer to the first array element,
						while \a size should specify the number of elements in the range.
		\param elementType The array element type.
		\param flags Modifiers for the array elements.
		\param ptr A pointer to the array instance, as described above.
		\param size Only used if \a arrayKind is \c AK_Range.
	 */
	void setArray( ArrayKind arrayKind, IType* elementType, uint32 flags, void* ptr, size_t size = 0 );

	//@}

	//@}
	//! \name Output Arguments
	//@{

	/*!
		Prepares a co::Any for use as an 'out' argument of the specified type.
		If the co::Any already contains a value, it will be preserved (if it's compatible).
		Otherwise, a default-constructed, temporary object will be created.
		\param paramType the 'out' parameter type.
		\throw co::Exception if the co::Any's current value is incompatible with 'paramType'.
		\sa makeIn()
	 */
	void makeOut( IType* paramType );

	/*!
		Should be used after a call to makeOut(), to return an argument to its 'in' condition.
		For instance, variables of type <tt>co::int8&</tt> will be dereferenced to a <tt>co::int8</tt>.
		\note This method does not raise exceptions.
	 */
	void makeIn();

	//@}

	/*!
		\name Temporary Objects
		A temporary object is created by calling one of the <em>createXXX()</em> methods.
		The object will last while its enclosing co::Any is alive, or until
		destroyObject() is called or another temporary object is created.
	 */
	//@{

	//! Returns whether this co::Any contains a temporary object.
	inline bool containsObject() const { return _state.objectKind != TK_NONE; }

	/*!
		Creates a temporary co::Any instance and makes this co::Any reference it.
		If the co::Any contains a variable at the time this method is called, it will
		be preserved in (moved to) the temporary co::Any instance. However, the co::Any
		must not contain a temporary object, or an exception will be thrown.
	 */
	Any& createAny();

	//! Creates a temporary std::string instance and makes this co::Any reference it.
	std::string& createString();

	/*!
		Creates a std::vector (or a co::RefVector, if \a elementType is an interface)
		with \a n default-constructed elements of type \a elementType, and sets this
		co::Any with a reference to the array.
	 */
	PseudoVector& createArray( IType* elementType, size_t n = 0 );

	/*!
		Swaps the temporary std::vector or co::RefVector contained in this
		co::Any (i.e. created with createArray()) with a std::vector or
		co::RefVector of the exact same type contained in another co::Any.
	 */
	void swapArray( const Any& other );

	/*!
		Creates an instance of the specified complex value \a type and makes
		this co::Any reference it.
	 */
	void* createComplexValue( IType* type );

	/*!
		Creates an instance of the complex value type \a T and makes this
		co::Any reference it.
	 */
	template<typename T>
	inline T& createComplexValue()
	{
		return *reinterpret_cast<T*>( createComplexValue( typeOf<T>::get() ) );
	}

	/*!
		Calls the appropriate destructors and releases any memory allocated
		for this co::Any's temporary object.
	 */
	void destroyObject();

	//@}

	//! Swaps the contents of two co::Any's.
	void swap( Any& other );

	//! Equality test operator.
	bool operator==( const Any& other ) const;

	//! Inequality test operator.
	inline bool operator!=( const Any& other ) const { return !( *this == other ); }

	//! Assignment operator.
	inline Any& operator=( const Any& other )
	{
		copy( other );
		return *this;
	}

private:
	//! Used by setVariable()/setArray().
	inline void setModifiers( uint32 flags );

	//! Raises co::IllegalCastException when the cast is impossible.
	void castFrom( const State& s );

	/*!
		Copies another co::Any's var. If the var points to an enclosed temporary
		object, also copies the object and updates the var.
	 */
	void copy( const Any& other );

private:
	/*
		Describes the variable stored in the co::Any.

		_state.objectKind holds the kind of temporary object currently allocated in
		the co::Any (if any). For complex values, it assumes special meanings:
			- TK_STRUCT: the CV is larger than INPLACE_CAPACITY and is heap-allocated.
			- TK_NATIVECLASS: the CV fits in INPLACE_CAPACITY and is kept in place.
	 */
	State _state;

	union TemporaryObjectData
	{
		State::Data data;
		uint8 stringArea[sizeof(std::string)];
		struct
		{
			uint8 vectorArea[sizeof(PseudoVector)];
			IReflector* reflector;
		}
		array;
		struct
		{
			union { void* ptr; uint8 inplaceArea[INPLACE_CAPACITY]; };
			IReflector* reflector;
		}
		complex;
	}
	_object;
};

typedef Any AnyValue;

#ifndef DOXYGEN
template<> struct kindOf<Any> { static const TypeKind kind = TK_ANY; };
inline void swap( Any& a, Any& b ) { a.swap( b ); }
#endif // DOXYGEN

} // namespace co

#ifndef DOXYGEN

// std::swap specialization for co::Any:
namespace std { template<> inline void swap( co::Any& a, co::Any& b ) { a.swap( b ); } }

// Prints the complete C++ type name of the variable stored in a __any::State.
CORAL_EXPORT std::ostream& operator<<( std::ostream& out, const co::__any::State& s );

#endif // DOXYGEN

/*!
	Prints out the variable type and data stored in a co::Any.
	\relates co::Any
 */
CORAL_EXPORT std::ostream& operator<<( std::ostream& out, const co::Any& a );

#endif // _CO_ANY_H_
