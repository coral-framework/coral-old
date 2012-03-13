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

	// IType of the variable; or, if this is an array, the element type
	IType* type;

	// convenience method to get 'type' downcasted to an IInterface
	inline IInterface* getInterface() const { return static_cast<IInterface*>( type ); }

	// only used for Ranges and C-strings
	uint32 size;

	// the kind of variable we're holding
	TypeKind kind : 8;

	// true if this variable is 'in', false if it is 'out'
	bool isIn;

	// reserved for use in co::Any:
	uint8 objectKind;
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

template<typename T> struct Value<T, TK_ANY> : public Ref<const T> {};
template<typename T> struct Value<T, TK_STRING> : public Ref<const T> {};
template<typename T> struct Value<T, TK_STRUCT> : public Ref<const T> {};
template<typename T> struct Value<T, TK_NATIVECLASS> : public Ref<const T> {};

template<typename T, bool isIn>
struct ValueBase
{
	inline static void tag( State& s )
	{
		static_assert( kindOf<T>::kind != TK_NONE, "unsupported type" );
		s.kind = kindOf<T>::kind;
		s.type = typeOf<T>::get();
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
struct Variable<T&> : public ValueBase<T, false>
{
	inline static void set( State& s, T& v ) { Ref<T>::set( s, v ); }
	inline static T& get( State& s ) { return Ref<T>::get( s ); }
};

template<typename T>
struct Variable<const T&> : public ValueBase<T, true>
{
	typedef Value<T, kindOf<T>::kind> ValueHelper;
	inline static void set( State& s, const T& v ) { ValueHelper::set( s, v ); }
	inline static const T& get( State& s ) { return ValueHelper::get( s ); }
};

template<typename T>
struct Variable<T* const&>
{
	inline static void tag( State& s )
	{
		static_assert( kindOf<T>::kind == TK_INTERFACE, "illegal pointer to a non-interface type" );
		s.kind = TK_INTERFACE;
		s.type = typeOf<T>::get();
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
	inline static void tag( State& s )
	{
		static_assert( kindOf<T>::kind == TK_INTERFACE, "illegal RefPtr to a non-interface type" );
		s.kind = TK_INTERFACE;
		s.type = typeOf<T>::get();
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

template<typename T, bool isIn>
struct ArrayBase
{
	inline static void tag( State& s )
	{
		Variable<T>::tag( s );
		s.kind = TK_ARRAY;
		s.isIn = isIn;
	}
};

template<typename T>
struct Variable<const Range<T>&> : public ArrayBase<T, true>
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
	: public ArrayBase<T, false>, public Ref<std::vector<T> > {};

template<typename T> struct Variable<RefVector<T>&>
	: public ArrayBase<T* const&, false>, public Ref<RefVector<T> > {};

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
	//! Alias to the struct that contains all info about a co::Any's var.
	typedef __any::State State;

	//! A 'neutral' std::vector type.
	typedef std::vector<uint8> PseudoVector;

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
	inline Any( T& var ) : _state()
	{
		set<T&>( var );
	}

	template<typename T>
	inline Any( const T& var ) : _state()
	{
		set<const T&>( var );
	}

	/*!
		\brief Constructor corresponding to a reflective set() call.
		Please, see set()'s documentation for more info.
	 */
	inline Any( bool isIn, IType* type, const void* ptr, size_t size = 0 ) : _state()
	{
		set( isIn, type, ptr, size );
	}

	//! Copy constructor.
	inline Any( Any& other ) : _state()
	{
		copy( other );
	}

	//! Constant copy constructor.
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

	//! Whether the stored variable is in the 'input' format.
	inline bool isIn() const { return _state.isIn; }

	//! Whether the stored variable is in the 'output' format.
	inline bool isOut() const { return !_state.isIn; }

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
	uint32 getSize() const;

	/*!
		Attempts to retrieve a stored variable, making the necessary casts whenever possible.

		\throw co::IllegalCastException if there is no valid cast from the stored variable's
				type to the requested type \a T.
	 */
	template<typename T>
	inline T get() const
	{
		State s;
		__any::Variable<T>::tag( s );
		cast( _state, s );
		return __any::Variable<T>::get( s );
	}

	template<typename T>
	inline void get( T& v ) const
	{
		State s;
		__any::Variable<T&>::tag( s );
		cast( _state, s );
		v = __any::Variable<T&>::get( s );
	}

	//! Provides full access to the internal state of a co::Any.
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
	inline void set( T& var )
	{
		__any::Variable<T&>::tag( _state );
		__any::Variable<T&>::set( _state, var );
	}

	template<typename T>
	inline void set( const T& var )
	{
		__any::Variable<const T&>::tag( _state );
		__any::Variable<const T&>::set( _state, var );
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
		Stores any variable supported by the Coral type system.

		\param isIn True if the variable is in the 'input' form, false if it's in the 'output' form.
		\param type Type of the variable.
		\param ptr Address of the variable.
		\param size Only used if the variable is an 'in array' or a C-string.
	 */
	void set( bool isIn, IType* type, const void* ptr, size_t size = 0 );

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
	static void cast( const State& from, State& to );

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
