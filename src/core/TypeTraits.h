/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPETRAITS_H_
#define _CO_TYPETRAITS_H_

#include <co/Platform.h>
#include <co/TypeKind.h>
#include <string>
#include <vector>

namespace co {

// forward declarations:
class Type;
class ArrayType;
class EnumType;
class ExceptionType;
class StructType;
class NativeClassType;
class InterfaceType;
class ComponentType;

/*!
	Array that maps a co::TypeKind to its Coral-style string representation.
	Use with caution, always make sure ( index >= 0 && index <= co::TK_COMPONENT ).
 */
extern CORAL_EXPORT const std::string TK_STRINGS[];

//! Just like co::TK_STRINGS, but contains C++ style strings (e.g. 'co::Any' instead of 'any').
extern CORAL_EXPORT const std::string TK_STRINGS_CPP[];


/****************************************************************************/
/* Type Traits: kindOf<T>::kind                                             */
/****************************************************************************/

template<typename>
struct kindOf
{
	static const TypeKind kind = TK_NONE;
};

//! Common implementation of co::kindOf<T>:
template<TypeKind k> struct kindOfBase { static const TypeKind kind = k; };

// specializations for basic types:
template<> struct kindOf<bool> : public kindOfBase<TK_BOOLEAN> {};
template<> struct kindOf<int8> : public kindOfBase<TK_INT8> {};
template<> struct kindOf<uint8> : public kindOfBase<TK_UINT8> {};
template<> struct kindOf<int16> : public kindOfBase<TK_INT16> {};
template<> struct kindOf<uint16> : public kindOfBase<TK_UINT16> {};
template<> struct kindOf<int32> : public kindOfBase<TK_INT32> {};
template<> struct kindOf<uint32> : public kindOfBase<TK_UINT32> {};
#if CORAL_POINTER_SIZE == 4
	// co::int32 is a 'long' on 32-bit systems, so we must also handle int's
	template<> struct kindOf<int> : public kindOfBase<TK_INT32> {};
	template<> struct kindOf<unsigned int> : public kindOfBase<TK_UINT32> {};
#endif
template<> struct kindOf<int64> : public kindOfBase<TK_INT64> {};
template<> struct kindOf<uint64>  : public kindOfBase<TK_UINT64> {};
template<> struct kindOf<float> : public kindOfBase<TK_FLOAT> {};
template<> struct kindOf<double> : public kindOfBase<TK_DOUBLE> {};
template<> struct kindOf<std::string> : public kindOfBase<TK_STRING> {};

// the specialization for co::TypeKind must go here to avoid mutual dependencies
template<> struct kindOf<TypeKind> : public kindOfBase<TK_ENUM> {};


/****************************************************************************/
/* co::nameOf<T>::get() returns the full name of a Coral type.              */
/****************************************************************************/

template<typename T>
struct nameOf
{
	static const char* get()
	{
		return TK_STRINGS[kindOf<T>::kind].c_str();
	}
};

// the specialization for co::TypeKind must go here to avoid mutual dependencies
template<> struct nameOf<TypeKind> { static const char* get() { return "co.TypeKind"; } };


/****************************************************************************/
/* co::typeOf<T>::get() returns the co::Type instance of a Coral type.      */
/****************************************************************************/

//! Common implementation of co::typeOf<T>:
template<typename T, typename R>
struct typeOfBase
{
	static R* get()
	{
		Type* type = getType( nameOf<T>::get() );
		assert( dynamic_cast<R*>( type ) );
		return static_cast<R*>( type );
	}
};

template<typename T>
struct typeOf : public typeOfBase<T, Type> {};


/****************************************************************************/
/* Type Traits - TR1/boost style type operators                             */
/****************************************************************************/

namespace traits {

template<typename T, T v>
struct IntegralConstant
{
	static const T					value = v;
	typedef T						ValueType;
	typedef IntegralConstant<T, v>	Type;
};

// The special 'true' and 'false' types:
typedef IntegralConstant<bool, true>	TrueType;
typedef IntegralConstant<bool, false>	FalseType;

// Whether two types are the same type:
template<typename, typename>
struct isSame : public FalseType {};

template<typename T>
struct isSame<T, T> : public TrueType {};

// Whether a type is 'const':
template<typename>
struct isConst : public FalseType {};

template<typename T>
struct isConst<T const> : public TrueType {};	

// Whether a type is a pointer:
template<typename>
struct isPointer : public FalseType {};

template<typename T>
struct isPointer<T*> : public TrueType {};

template<typename T>
struct isPointer<T* const> : public TrueType {};

// Whether a type is a reference:
template<typename>
struct isReference : public FalseType {};

template<typename T>
struct isReference<T&> : public TrueType {};

// Removes 'const' from a type:
template<typename T>
struct removeConst { typedef T Type; };

template<typename T>
struct removeConst<T const> { typedef T Type; };

// Removes a pointer ('*') from a type:
template<typename T>
struct removePointer { typedef T Type; };

template<typename T>
struct removePointer<T*> { typedef T Type; };

template<typename T>
struct removePointer<T* const> { typedef T Type; };

// Removes a reference ('&') from a type:
template<typename T>
struct removeReference { typedef T Type; };

template<typename T>
struct removeReference<T&> { typedef T Type; };

// Whether an interface 'Itf' inherits ambiguously from a certain 'Base' interface:
template<typename, typename>
struct hasAmbiguousBase : public FalseType {};

// Indicates which 'Super'-interface should be used to disambiguate a certain 'Base' interface of 'Itf'.
template<typename Itf, typename Base>
struct disambiguateBase {};

//! Utility struct to easily access information about a Coral type.
template<typename T>
struct get
{
	typedef T												Type;
	typedef typename removeReference<T>::Type				ReferencedType;
	typedef typename removeConst<ReferencedType>::Type		ValueType;
	typedef typename removePointer<ReferencedType>::Type	PointedType;
	typedef typename removeConst<PointedType>::Type			CoreType;

	static const bool isConst = isConst<PointedType>::value;
	static const bool isPointer = isPointer<ReferencedType>::value;
	static const bool isPointerConst = co::traits::get<T>::isPointer && co::traits::isConst<ReferencedType>::value;
	static const bool isReference = isReference<T>::value;

	static const TypeKind kind = kindOf<CoreType>::kind;
};

} // namespace traits


/****************************************************************************/
/* co::disambiguate<Base>( T* ) returns a Base* for an ambiguous base of T. */
/****************************************************************************/

template<typename Itf, typename Base, bool isAmbiguous>
struct __disambiguate {};

template<typename Itf, typename Base>
struct __disambiguate<Itf, Base, false> { typedef Itf Ancestor; };

template<typename Itf, typename Base>
struct __disambiguate<Itf, Base, true>
{
	typedef typename traits::disambiguateBase<Itf, Base>::Super Super;
	typedef typename __disambiguate<Super, Base, traits::hasAmbiguousBase<Super, Base>::value>::Ancestor Ancestor;
};

template<typename Base, typename T>
inline Base* disambiguate( T* p )
{
	return static_cast<typename __disambiguate<T, Base, traits::hasAmbiguousBase<T, Base>::value>::Ancestor*>( p );
}


/****************************************************************************/
/* All type-traits definitions related to std::vector are located below     */
/****************************************************************************/

template<typename T>
struct kindOf<std::vector<T> > : public kindOfBase<TK_ARRAY> {};

//! Common implementation of co::nameOf<T> for arrays:
template<typename ET>
struct nameOfArrayBase
{
	typedef typename traits::get<ET> ETT; // element type traits
	static const char* get()
	{
		static const std::string s_name( std::string( nameOf<typename ETT::CoreType>::get() ) + "[]" );		
		return s_name.c_str();
	}
};

template<typename T>
struct nameOf<std::vector<T> > : public nameOfArrayBase<T> {};

//! Common implementation of co::typeOf<T> for arrays:
template<typename ET>
struct typeOfArrayBase
{
	static ArrayType* get()
	{
		co::Type* type = getType( nameOf<std::vector<ET> >::get() );
		assert( dynamic_cast<ArrayType*>( type ) );
		return static_cast<ArrayType*>( type );
	}
};

template<typename T>
struct typeOf<std::vector<T> > : public typeOfArrayBase<T> {};

} // namespace co

#endif
