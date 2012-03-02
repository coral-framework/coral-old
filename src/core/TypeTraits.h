/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_TYPETRAITS_H_
#define _CO_TYPETRAITS_H_

#include <co/Platform.h>
#include <co/TypeKind.h>
#include <algorithm>
#include <string>
#include <vector>

namespace co {

// Forward Decls:
class IEnum;
class IType;
class IArray;
class IObject;
class IStruct;
class IService;
class IException;
class IInterface;
class IComponent;
class INativeClass;
template<typename T> class Range;
template<typename T> class RefPtr;
template<typename T> class RefVector;

/*!
	Array that maps a TypeKind to its Coral-style string representation.
	Use with caution, always make sure ( index >= 0 && index <= co::TK_COMPONENT ).
 */
extern CORAL_EXPORT const std::string TK_STRINGS[];

//! Just like co::TK_STRINGS, but contains C++ style strings (e.g. 'co::Any' instead of 'any').
extern CORAL_EXPORT const std::string TK_STRINGS_CPP[];


/****************************************************************************/
/* Internal Helper Functions (to avoid type dependencies in template code)  */
/****************************************************************************/

#ifndef DOXYGEN

// Just like co::getType() but takes a C-string instead of a std::string.
CORAL_EXPORT IType* getTypeByLiteralName( const char* fullName );

// Returns the kind of a 'type'.
CORAL_EXPORT TypeKind getKind( IType* type );

// Gets a service of the given 'type', provided by 'object'.
CORAL_EXPORT IService* getServiceByType( IObject* object, IInterface* type );

// Gets a service from 'object' by its port name.
CORAL_EXPORT IService* getServiceByName( IObject* object, const std::string& portName );

// Binds a 'service' to the receptacle identified by 'receptacleName' in 'object'.
CORAL_EXPORT void setServiceByName( IObject* object, const std::string& receptacleName, IService* service );

#endif


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
template<> struct kindOf<int64> : public kindOfBase<TK_INT64> {};
template<> struct kindOf<uint64>  : public kindOfBase<TK_UINT64> {};
template<> struct kindOf<float> : public kindOfBase<TK_FLOAT> {};
template<> struct kindOf<double> : public kindOfBase<TK_DOUBLE> {};
template<> struct kindOf<std::string> : public kindOfBase<TK_STRING> {};

// specialization for container types:
template<typename T> struct kindOf<Range<T> > : public kindOfBase<TK_ARRAY> {};
template<typename T> struct kindOf<RefPtr<T> > : public kindOf<T> {};
template<typename T> struct kindOf<RefVector<T> > : public kindOfBase<TK_ARRAY> {};
template<typename T> struct kindOf<std::vector<T> > : public kindOfBase<TK_ARRAY> {};

// specialization for co::TypeKind (must go here to avoid cyclic dependencies)
template<> struct kindOf<TypeKind> : public kindOfBase<TK_ENUM> {};

	
/****************************************************************************/
/* Template mapping of each TypeKind to its descriptor interface            */
/****************************************************************************/

template<TypeKind> struct typeDescriptorFor { typedef IType Type; };
template<> struct typeDescriptorFor<TK_ARRAY> { typedef IArray Type; };
template<> struct typeDescriptorFor<TK_ENUM> { typedef IEnum Type; };
template<> struct typeDescriptorFor<TK_EXCEPTION> { typedef IException Type; };
template<> struct typeDescriptorFor<TK_STRUCT> { typedef IStruct Type; };
template<> struct typeDescriptorFor<TK_NATIVECLASS> { typedef INativeClass Type; };
template<> struct typeDescriptorFor<TK_INTERFACE> { typedef IInterface Type; };
template<> struct typeDescriptorFor<TK_COMPONENT> { typedef IComponent Type; };


/****************************************************************************/
/* Type Traits - TR1/boost style type operators                             */
/****************************************************************************/

namespace traits {

// The special 'false' and 'true' types:
struct FalseType { static const bool value = false; };
struct TrueType { static const bool value = true; FalseType _[2]; };

// Whether two types are the same type:
template<typename, typename> struct isSame : public FalseType {};
template<typename T> struct isSame<T, T> : public TrueType {};

// Whether a type is 'const':
template<typename> struct isConst : public FalseType {};
template<typename T> struct isConst<T const> : public TrueType {};

// Whether a type is a pointer:
template<typename> struct isPointer : public FalseType {};
template<typename T> struct isPointer<T*> : public TrueType {};
template<typename T> struct isPointer<T* const> : public TrueType {};

// Whether a type is a reference:
template<typename> struct isReference : public FalseType {};
template<typename T> struct isReference<T&> : public TrueType {};

// Removes 'const' from a type:
template<typename T> struct removeConst { typedef T Type; };
template<typename T> struct removeConst<T const> { typedef T Type; };

// Removes a pointer ('*') from a type:
template<typename T> struct removePointer { typedef T Type; };
template<typename T> struct removePointer<T*> { typedef T Type; };
template<typename T> struct removePointer<T* const> { typedef T Type; };

// Removes a reference ('&') from a type:
template<typename T> struct removeReference { typedef T Type; };
template<typename T> struct removeReference<T&> { typedef T Type; };

// Whether a type D is a subtype of B.
template<typename D, typename B>
struct isSubTypeOf
{
	static TrueType test( const B* const );
	static FalseType test( ... );
	static const bool value = ( sizeof(test((D*)0)) == sizeof(TrueType) );
};

//! Utility struct to easily access information about a Coral type.
template<typename T>
struct get
{
	typedef T												Type;
	typedef typename removeReference<T>::Type				ReferencedType;
	typedef typename removeConst<ReferencedType>::Type		ValueType;
	typedef typename removePointer<ReferencedType>::Type	PointedType;
	typedef typename removeConst<PointedType>::Type			CoreType;

	static const bool isConst = co::traits::isConst<PointedType>::value;
	static const bool isPointer = co::traits::isPointer<ReferencedType>::value;
	static const bool isPointerConst = co::traits::get<T>::isPointer
							&& co::traits::isConst<ReferencedType>::value;
	static const bool isReference = co::traits::isReference<T>::value;

	static const TypeKind kind = kindOf<CoreType>::kind;
};

} // namespace traits


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

// specialization for array types:
template<typename T> struct nameOf<Range<T> > : public nameOfArrayBase<T> {};
template<typename T> struct nameOf<RefVector<T> > : public nameOfArrayBase<T> {};
template<typename T> struct nameOf<std::vector<T> > : public nameOfArrayBase<T> {};

// specialization for co::TypeKind (must go here to avoid cyclic dependencies)
template<> struct nameOf<TypeKind> { static const char* get() { return "co.TypeKind"; } };


/****************************************************************************/
/* co::typeOf<T>::get() returns the co::IType instance of a Coral type      */
/****************************************************************************/

template<typename T>
struct typeOf
{
	static const TypeKind kind = kindOf<T>::kind;
	typedef typename typeDescriptorFor<kind>::Type Descriptor;

	static Descriptor* get()
	{
		IType* type = getTypeByLiteralName( nameOf<T>::get() );
		assert( getKind( type ) == kind );
		return static_cast<Descriptor*>( type );
	}
};


/****************************************************************************/
/* Basic RTTI functions                                                     */
/****************************************************************************/

/*!
	Returns whether a \a service is of a given \a type.
	This method always returns \c true if \a service is NULL.
 */
CORAL_EXPORT bool isA( IService* service, IInterface* type );

template<typename SubType>
inline bool isA( IService* service )
{
	return isA( service, typeOf<SubType>::get() );
}

/*!
	Raises an IllegalCastException if \a service if not of a given \a type.
	No exception is raised if \a service is NULL.
 */
CORAL_EXPORT void ensureIsA( IService* service, IInterface* type );

/*!
	Downcasts a service, raising an IllegalCastException on failure.
	The cast always succeeds for NULL pointers (result is a NULL pointer).
	\throw IllegalCastException if the service is not of the requested type.
 */
template<typename SubType>
inline SubType* cast( IService* service )
{
	ensureIsA( service, typeOf<SubType>::get() );
	return static_cast<SubType*>( service );
}

} // namespace co

#endif
