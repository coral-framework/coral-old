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

class AnyValue;

#ifndef DOXYGEN

//! Private namespace with auxiliary code for co::Any.
namespace __any {

union Data
{
	// Storage for primitives:
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
	co::uint8* bytes;
	std::string* str;
	IService* service;

	// Constructors
	Data() : d( 0 ) {;}
	Data( bool b ) : b( b ) {;}
	Data( int8 i8 ) : i8( i8 ) {;}
	Data( uint8 u8 ) : u8( u8 ) {;}
	Data( int16 i16 ) : i16( i16 ) {;}
	Data( uint16 u16 ) : u16( u16 ) {;}
	Data( int32 i32 ) : i32( i32 ) {;}
	Data( uint32 u32 ) : u32( u32 ) {;}
	Data( int64 i64 ) : i64( i64 ) {;}
	Data( uint64 u64 ) : u64( u64 ) {;}
	Data( float f ) : f( f ) {;}
	Data( double d ) : d( d ) {;}
	Data( const void* ptr ) : cptr( ptr ) {;}
};

struct State
{
	IType* type;	// Variable Type
	bool isIn : 1;	// True if the var is 'in'; False if it is 'out'
	size_t size		// Only used for 'in' arrays and C-strings
#if CORAL_POINTER_SIZE == 4
		: 31;
#else
		: 63;
#endif
	Data data;		// Storage for primitives and pointers

	// default constructor
	State() : type() {;}

	// 'out' variable constructor
	State( IType* type, const void* ptr )
		: type( type ), isIn( false ), data( ptr ) {;}
};

template<typename T>
struct Ref
{
	inline static void set( State& s, T& v ) { s.data.cptr = &v; }
	inline static T& get( State& s ) { return *reinterpret_cast<T*>( s.data.ptr ); }
};

template<typename T, TypeKind kind>
struct Value
{
	inline static void set( State& s, const T& v ) { *reinterpret_cast<T*>( &s.data.ptr ) = v; }
	inline static const T& get( State& s ) { return *reinterpret_cast<T*>( &s.data.ptr ); }
};

template<typename T> struct Value<T, TK_ANY> : public Ref<const AnyValue> {};
template<typename T> struct Value<T, TK_STRING> : public Ref<const T> {};
template<typename T> struct Value<T, TK_STRUCT> : public Ref<const T> {};
template<typename T> struct Value<T, TK_NATIVECLASS> : public Ref<const T> {};

template<typename T, bool isIn>
struct Tag
{
	typedef typeOf<T> VT;
	inline static void tag( State& s )
	{
		static_assert( VT::kind != TK_NONE, "unsupported type" );
		s.type = VT::get();
		s.isIn = isIn;
	}
};

template<typename T>
struct Variable
{
	inline static void tag( State& s ) { Variable<const T&>::tag( s ); }
	inline static void set( State& s, const T& v ) { Variable<const T&>::set( s, v ); }
	inline static T get( State& s ) { return Variable<const T&>::get( s ); }
};

template<typename T>
struct Variable<T&> : public Tag<T, false>
{
	inline static void set( State& s, T& v ) { Ref<T>::set( s, v ); }
	inline static T& get( State& s ) { return Ref<T>::get( s ); }
};

template<typename T>
struct Variable<const T&> : public Tag<T, true>
{
	typedef Value<T, kindOf<T>::kind> ValueHelper;
	inline static void set( State& s, const T& v ) { ValueHelper::set( s, v ); }
	inline static const T& get( State& s ) { return ValueHelper::get( s ); }
};

template<typename T>
struct Variable<T* const&>
{
	typedef typeOf<T> IT;
	inline static void tag( State& s )
	{
		static_assert( IT::kind == TK_INTERFACE, "illegal pointer to a non-interface type" );
		s.type = IT::get();
		s.isIn = true;
	}
	inline static void set( State& s, T* v )
	{
		if( v ) s.type = v->getInterface();
		s.data.service = v;
	}
	inline static T* get( State& s ) { return static_cast<T*>( s.data.service ); }
};

template<typename T>
struct Variable<T*&> : public Variable<T* const&> {};

template<typename T>
struct Variable<RefPtr<T>&>
{
	typedef typeOf<T> IT;
	inline static void tag( State& s )
	{
		static_assert( IT::kind == TK_INTERFACE, "illegal RefPtr to a non-interface type" );
		s.type = IT::get();
		s.isIn = false;
	}
	inline static void set( State& s, RefPtr<T>& v ) { s.data.ptr = &v; }
	inline static RefPtr<T>& get( State& s ) { return *reinterpret_cast<RefPtr<T>*>( s.data.ptr ); }
};

template<typename T>
struct Variable<const RefPtr<T>&>
{
	inline static void tag( State& ) { static_assert( sizeof(T)<0, "illegal const RefPtr" ); }
};
 
template<typename T>
struct Variable<const T* const&>
{
	inline static void tag( State& ) { static_assert( sizeof(T)<0, "illegal const pointer" ); }
};

template<typename T>
struct Variable<const T*&> : public Variable<const T* const&> {};

template<typename T>
struct Variable<const Range<T>&> : public Tag<Range<T>, true>
{
	inline static void set( State& s, const Range<T>& v )
	{
		s.data.cptr = v.getStart();
		s.size = v.getSize();
	}
	inline static Range<T> get( State& s )
	{
		return Range<T>( reinterpret_cast<T*>( s.data.ptr ), s.size );
	}
};

template<typename T> struct Variable<Range<T>&> : public Variable<const Range<T>&> {};

template<typename T> struct Variable<std::vector<T>&>
	: public Tag<std::vector<T>, false>, public Ref<std::vector<T> > {};

template<typename T> struct Variable<RefVector<T>&>
	: public Tag<RefVector<T>, false>, public Ref<RefVector<T> > {};

template<typename T>
struct Variable<const std::vector<T>&>
{
	inline static void tag( State& ) { static_assert( sizeof(T)<0, "illegal const std::vector" ); }
};

template<typename T>
struct Variable<const RefVector<T>&>
{
	inline static void tag( State& ) { static_assert( sizeof(T)<0, "illegal const co::RefVector" ); }
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
		For instance, an array passed as a <tt>co::Range<std::string></tt> is completely immutable &mdash;
		as opposed to a <tt>std::vector<std::string></tt>, which is fully mutable.
		\par
		Arrays represented by \c std::vectors or \c co::RefVectors must always be passed and retrieved
		<b>by reference</b>, while \c co::Ranges must always be passed and retrieved <b>by value</b>.
		\par
		Arrays must generally be retrieved by the exact same type they were passed. However, when
		retrieving \c co::Ranges the following coercion rules apply:
		  - It is possible to retrieve a <tt>co::Range<\e ValueType></tt> from an array passed as a
				<tt>std::vector<\e ValueType></tt>.
		  - It is possible to retrieve a <tt>co::Range<\e SuperType*></tt> from an array
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
	//! Variable state descriptor.
	__any::State state;

	//! Creates a null co::Any.
	inline Any() : state() {;}

	//! Creates a reference to any non-const Coral variable.
	template<typename T>
	inline Any( T& var ) { set<T&>( var ); }

	//! Creates a reference to any const Coral variable.
	template<typename T>
	inline Any( const T& var ) { set<const T&>( var ); }

	//! Creates a reference to any variable reflectively. \see set()
	inline Any( bool isIn, IType* type, const void* addr, size_t size = 0 )
	{
		set( isIn, type, addr, size );
	}

	//! Copy constructor.
	inline Any( Any& other ) : state( other.state ) {;}

	//! Constant copy constructor.
	inline Any( const Any& other ) : state( other.state ) {;}

	// Creates a co::Any from a state descriptor.
	inline Any( const __any::State& state ) : state( state ) {;}

	//! Destructor.
	inline ~Any() {;}

	//! Clears the co::Any (makes it null).
	inline void clear() { state.type = NULL; }

	//! \name Variable Introspection
	//@{

	//! Whether this co::Any is empty (null).
	inline bool isNull() const { return state.type == NULL; }

	//! Whether this co::Any references something (isn't null).
	inline bool isValid() const { return state.type != NULL; }

	//! Returns the type of variable in this co::Any (or NULL if it's empty).
	inline IType* getType() const { return state.type; }

	//! Whether the stored variable is in the 'input' format.
	inline bool isIn() const { return state.isIn; }

	//! Whether the stored variable is in the 'output' format.
	inline bool isOut() const { return !state.isIn; }

	//! Whether the stored variable is of the given type.
	bool isA( IType* type ) const;

	//! Returns the equivalent variable in 'input' format.
	Any asIn() const;

	/*!
		\brief Returns the number of elements contained in this co::Any.
		For arrays, this is the number of elements in the array; for other
		types this is always 1.
	 */
	size_t getCount() const;

	/*!
		\brief Returns the individual size of the elements in this co::Any.
		For arrays, this is the size of each array element; for other types
		this is the size reported by the type's reflector.
	 */
	size_t getElementSize() const;

	/*!
		Attempts to retrieve a stored variable, making the necessary casts whenever possible.

		\throw co::IllegalCastException if there is no valid cast from the stored variable's
				type to the requested type \a T.
	 */
	template<typename T>
	inline T get() const
	{
		__any::State s;
		__any::Variable<T>::tag( s );
		cast( s );
		return __any::Variable<T>::get( s );
	}

	template<typename T>
	inline void get( T& v ) const
	{
		__any::State s;
		__any::Variable<T&>::tag( s );
		cast( s );
		v = __any::Variable<T&>::get( s );
	}

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
	inline void setIn( const T& var )
	{
		__any::Variable<const T&>::tag( state );
		__any::Variable<const T&>::set( state, var );
	}

	template<typename T>
	inline void set( const T& var )
	{
		setIn<T>( var );
	}

	template<typename T>
	inline void set( T& var )
	{
		__any::Variable<T&>::tag( state );
		__any::Variable<T&>::set( state, var );
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
		Points to any variable supported by the Coral type system.
		\param isIn Whether the variable is in the 'input' or 'output' form.
		\param type Type of the variable.
		\param addr Address of the value associated with the variable.
		\param size Only used if the variable is an input array or a C-string.
	 */
	void set( bool isIn, IType* type, const void* addr, size_t size = 0 );

	//@}

	/*!
		Stores a \a value onto the location pointed to by an output variable.
		If necessary, the \a value is converted to the output variable's
		type when copied to its destination.
		\throw IllegalStateException if this is not an output variable.
		\throw IllegalCastException if \a value cannot be converted to
					the output variable's type.
	 */
	void put( const Any& value ) const;

	//! Swaps the contents of two co::Any's.
	inline void swap( Any& other )
	{
		__any::State tmp( state );
		state = other.state;
		other.state = tmp;
	}

	/*!
		Swaps the vector pointers of two co::Any's containing
		'out arrays' of the same type.
	 */
	void swapArrays( const Any& other ) const;

	//! Equality test operator.
	bool operator==( const Any& other ) const;

	//! Inequality test operator.
	inline bool operator!=( const Any& other ) const { return !( *this == other ); }

	//! Assignment operator.
	inline Any& operator=( const Any& other )
	{
		state = other.state;
		return *this;
	}

	inline Any& operator=( Any& other )
	{
		state = other.state;
		return *this;
	}

	//! Index operator for reflective access to array elements.
	Any operator[]( size_t index ) const;

protected:
	// Raises co::IllegalCastException when the cast is impossible.
	void cast( __any::State& to ) const;
};

/*!
	????
 */
class CORAL_EXPORT AnyValue
{
public:
	//! Default constructor.
	AnyValue() : _type( NULL ) {;}

	template<typename T>
	inline AnyValue( const T& var ) : _type( NULL )
	{
		copy( Any( var ) );
	}

	inline AnyValue( const Any& other ) : _type( NULL )
	{
		copy( other );
	}

	//! Copy constructor.
	inline AnyValue( const AnyValue& other ) : _type( NULL )
	{
		copy( other.getAny() );
	}

	//! Destructor.
	inline ~AnyValue() { clear(); }

	//! Whether this co::Any is empty (null).
	inline bool isNull() const { return _type == NULL; }

	//! Whether this co::AnyValue contains a value (isn't null).
	inline bool isValid() const { return _type != NULL; }

	//! Returns an 'out' co::Any that references this co::AnyValue's value.
	Any getAny() const;

	template<typename T>
	inline T get() const
	{
		return getAny().get<T>();
	}

	template<typename T>
	inline void get( T& v ) const
	{
		return getAny().get<T>( v );
	}

	/*!
		Destroys any stored value and makes this any null.
	 */
	void clear();

	template<typename T>
	inline void set( const T& var )
	{
		copy( Any( var ) );
	}

	/*!
		Creates a default-constructed instance of the given \a type.
		If the type is an array, it's created with \a n elements.
		\throw ???
	 */
	void* create( IType* type, size_t n = 1 );

	/*!
		Creates an instance of the complex value type \a T and makes this
		co::Any reference it.
	 */
	template<typename T>
	inline T& create()
	{
		return *reinterpret_cast<T*>( create( typeOf<T>::get() ) );
	}

	/*!
		Converts the current value to the given \a type.
		If the co::AnyValue is null this method works like create().
		\throw IllegalCastException if the value cannot be converted
					to the target \a type.
	 */
	void convert( IType* type );

	//! Swaps the contents of two co::AnyValue's.
	void swap( AnyValue& other );

	//! Equality test operator.
	inline bool operator==( const AnyValue& other ) const
	{
		return getAny() == other.getAny();
	}
	
	//! Inequality test operator.
	inline bool operator!=( const AnyValue& other ) const
	{
		return !( *this == other );
	}

	//! Assignment operator.
	inline AnyValue& operator=( const AnyValue& other )
	{
		copy( other.getAny() );
		return *this;
	}

	template<typename T>
	inline AnyValue& operator=( const T& var )
	{
		set<T>( var );
		return *this;
	}

private:
	void copy( const Any& any );

private:
	IType* _type;
	__any::Data _data;
};

#ifndef DOXYGEN
template<> struct kindOf<Any> { static const TypeKind kind = TK_ANY; };
template<> struct kindOf<AnyValue> { static const TypeKind kind = TK_ANY; };
inline void swap( Any& a, Any& b ) { a.swap( b ); }
inline void swap( AnyValue& a, AnyValue& b ) { a.swap( b ); }
#endif // DOXYGEN

} // namespace co

#ifndef DOXYGEN

// std::swap specialization for co::Any:
namespace std {
	template<> inline void swap( co::Any& a, co::Any& b ) { a.swap( b ); }
	template<> inline void swap( co::AnyValue& a, co::AnyValue& b ) { a.swap( b ); }
} // namespace std

// Outputs the qualified type name of the variable in a co::__any::State.
CORAL_EXPORT std::ostream& operator<<( std::ostream& out, const co::__any::State& s );

#endif // DOXYGEN

/*!
	Outputs the variable type and value stored in a co::Any.
	\relates co::Any
 */
CORAL_EXPORT std::ostream& operator<<( std::ostream& out, const co::Any& a );

/*!
	Outputs the variable type and value stored in a co::AnyValue.
	\relates co::AnyValue
 */
inline std::ostream& operator<<( std::ostream& out, const co::AnyValue& v )
{
	return out << v.getAny();
}

#endif // _CO_ANY_H_
