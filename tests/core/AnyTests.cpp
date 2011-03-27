/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <gtest/gtest.h>

#include <co/Uuid.h>
#include <co/Any.h>
#include <co/ISystem.h>
#include <co/IModule.h>
#include <co/CSLError.h>
#include <co/IObject.h>
#include <co/INamespace.h>
#include <co/IMember.h>
#include <co/IStruct.h>
#include <co/ITypeManager.h>
#include <co/IInterface.h>
#include <co/INativeClass.h>
#include <co/IRecordType.h>
#include <co/IllegalCastException.h>

/****************************************************************************
 *	Performance / Portability Tests
 ****************************************************************************/

TEST( AnyTests, sizeOf )
{
	// we assume size_t has the size of a pointer
	EXPECT_EQ( CORAL_POINTER_SIZE, sizeof(void*) );
	EXPECT_EQ( sizeof(void*), sizeof(size_t) );

	// make sure a co::Any is able to store all basic types inplace
	EXPECT_GE( co::Any::INPLACE_CAPACITY, sizeof(co::Any::State::Data) );
	EXPECT_GE( co::Any::INPLACE_CAPACITY, sizeof(std::string) );
	EXPECT_GE( co::Any::INPLACE_CAPACITY, sizeof(sizeof(co::Any::PseudoVector)) );

	/*
		Make sure sizeof(co::Any::State) is as expected.
		A co::Any::State instance contains a double, a pointer, a uint32 and 2 bytes.
	 */
#if CORAL_POINTER_SIZE == 4
	#if defined(CORAL_OS_UNIX)
		// 32-bit system with 4-byte alignment
		EXPECT_EQ( 20, sizeof(co::Any::State) );
		EXPECT_EQ( 20 + co::Any::INPLACE_CAPACITY + sizeof(size_t), sizeof(co::Any) );
	#else
		// 32-bit system with 8-byte alignment
		EXPECT_EQ( 24, sizeof(co::Any::State) );
		EXPECT_EQ( 24 + co::Any::INPLACE_CAPACITY + sizeof(size_t) + 4, sizeof(co::Any) );
	#endif
#elif CORAL_POINTER_SIZE == 8
	// 64-bit system with 8-byte alignment
	EXPECT_EQ( 24, sizeof(co::Any::State) );
	EXPECT_EQ( 24 + co::Any::INPLACE_CAPACITY + sizeof(size_t), sizeof(co::Any) );
#else
#error Huh, pointers are neither 32 nor 64 bit long?
#endif
}

TEST( AnyTests, stdVectorMemoryLayout )
{
	/*
		In co::Any we assume some truths about the memory layout of std::vectors.
		This test verifies whether the assumptions are true for the local STL distro.
	
		Basically, the assumption is that the std::vector computes its size() by subtracting
		two pointers (one to the start, another past the end of the array). This is a fairly
		standard implementation choice for std::vectors, and allows Coral to find out how much
		memory is allocated by a std::vector in run-time without knowing its template type.
	 
		A derived assumption is that all std::vectors have the same size.
	 */

	size_t s1 = sizeof(std::vector<co::uint8>); 
	size_t s2 = sizeof(co::RefVector<co::IService>);
	ASSERT_EQ( s1, s2 );

	std::vector<double> dblVector( 8 );
	ASSERT_EQ( 8, dblVector.size() );

	std::vector<co::uint8>& bytesVector = *reinterpret_cast<std::vector<co::uint8>*>( &dblVector );
	ASSERT_EQ( 64, bytesVector.size() );
}

/*****************************************************************************
 *	A 'constructor' test checks whether the co::Any is detecting the correct
 *	type for a variable passed to its constructor. It may also include basic
 *	retrieval tests for the values passed to the constructor.
 *****************************************************************************/

void EXPECT_ANY_STREQ( const co::Any& any, const char* str )
{
	std::stringstream ss;
	ss << any;
	EXPECT_STREQ( str, ss.str().c_str() );
}

void EXPECT_ANY_TYPE_STREQ( const co::Any& any, const char* str )
{
	std::stringstream ss;
	ss << any.getState();
	EXPECT_STREQ( str, ss.str().c_str() );
}

template<typename T>
void constructorValueTest( const co::TypeKind kind, const T& sampleValue, co::IType* expectedType )
{
	T v = sampleValue;
	co::Any a1( v );
	EXPECT_TRUE( a1.getKind() == kind );
	EXPECT_FALSE( a1.isConst() );
	EXPECT_FALSE( a1.isPointer() );
	EXPECT_FALSE( a1.isPointerConst() );
	EXPECT_FALSE( a1.isReference() );
	EXPECT_EQ( sizeof(T), a1.getSize() );
	EXPECT_EQ( expectedType, a1.getType() );
	EXPECT_EQ( v, a1.get<T>() );

	const T cv = sampleValue;
	co::Any a2;
	a2.set<const T>( cv );
	EXPECT_TRUE( a2.getKind() == kind );
	EXPECT_TRUE( a2.isConst() );
	EXPECT_FALSE( a2.isPointer() );
	EXPECT_FALSE( a2.isPointerConst() );
	EXPECT_FALSE( a2.isReference() );
	EXPECT_EQ( sizeof(T), a2.getSize() );
	EXPECT_EQ( expectedType, a2.getType() );
	EXPECT_EQ( cv, a2.get<const T>() );
}

template<typename T>
void constructorPtrTest( const co::TypeKind kind, T* sampleValue, co::IType* expectedType )
{
	T* p = sampleValue;
	co::Any a3( p );
	EXPECT_TRUE( a3.getKind() == kind );
	EXPECT_FALSE( a3.isConst() );
	EXPECT_TRUE( a3.isPointer() );
	EXPECT_FALSE( a3.isPointerConst() );
	EXPECT_FALSE( a3.isReference() );
	EXPECT_EQ( sizeof(void*), a3.getSize() );
	EXPECT_EQ( expectedType, a3.getType() );
	EXPECT_EQ( p, a3.get<T*>() );
	
	const T* cp = sampleValue;
	co::Any a4( cp );
	EXPECT_TRUE( a4.getKind() == kind );
	EXPECT_TRUE( a4.isConst() );
	EXPECT_TRUE( a4.isPointer() );
	EXPECT_FALSE( a4.isPointerConst() );
	EXPECT_FALSE( a4.isReference() );
	EXPECT_EQ( sizeof(void*), a4.getSize() );
	EXPECT_EQ( expectedType, a4.getType() );
	EXPECT_EQ( cp, a4.get<const T*>() );
	
	T* const pc = sampleValue;
	co::Any a5;
	a5.set<T* const>( pc );
	EXPECT_TRUE( a5.getKind() == kind );
	EXPECT_FALSE( a5.isConst() );
	EXPECT_TRUE( a5.isPointer() );
	EXPECT_TRUE( a5.isPointerConst() );
	EXPECT_FALSE( a5.isReference() );
	EXPECT_EQ( sizeof(void*), a5.getSize() );
	EXPECT_EQ( expectedType, a5.getType() );
	EXPECT_EQ( pc, a5.get<T* const>() );
	
	const T* const cpc = sampleValue;
	co::Any a6;
	a6.set<const T* const>( cpc );
	EXPECT_TRUE( a6.getKind() == kind );
	EXPECT_TRUE( a6.isConst() );
	EXPECT_TRUE( a6.isPointer() );
	EXPECT_TRUE( a6.isPointerConst() );
	EXPECT_FALSE( a6.isReference() );
	EXPECT_EQ( sizeof(void*), a6.getSize() );
	EXPECT_EQ( expectedType, a6.getType() );
	EXPECT_EQ( cpc, a6.get<const T* const>() );
	
	T*& pr = p;
	co::Any a9;
	a9.set<T*&>( pr );
	EXPECT_TRUE( a9.getKind() == kind );
	EXPECT_FALSE( a9.isConst() );
	EXPECT_TRUE( a9.isPointer() );
	EXPECT_FALSE( a9.isPointerConst() );
	EXPECT_TRUE( a9.isReference() );
	EXPECT_EQ( sizeof(void*), a9.getSize() );
	EXPECT_EQ( expectedType, a9.getType() );
	EXPECT_EQ( pr, a9.get<T*&>() );
	
	const T*& cpr = cp;
	co::Any a10;
	a10.set<const T*&>( cpr );
	EXPECT_TRUE( a10.getKind() == kind );
	EXPECT_TRUE( a10.isConst() );
	EXPECT_TRUE( a10.isPointer() );
	EXPECT_FALSE( a10.isPointerConst() );
	EXPECT_TRUE( a10.isReference() );
	EXPECT_EQ( sizeof(void*), a10.getSize() );
	EXPECT_EQ( expectedType, a10.getType() );
	EXPECT_EQ( cpr, a10.get<const T*&>() );
}

template<typename T>
void constructorRefTest( const co::TypeKind kind, const T& sampleValue, co::IType* expectedType )
{
	T v = sampleValue;
	
	constructorPtrTest<T>( kind, &v, expectedType );

	T& r = v;
	co::Any a7;
	a7.set<T&>( r );
	EXPECT_TRUE( a7.getKind() == kind );
	EXPECT_FALSE( a7.isConst() );
	EXPECT_FALSE( a7.isPointer() );
	EXPECT_FALSE( a7.isPointerConst() );
	EXPECT_TRUE( a7.isReference() );
	EXPECT_EQ( sizeof(T), a7.getSize() );
	EXPECT_EQ( expectedType, a7.getType() );
	EXPECT_EQ( r, a7.get<T&>() );

	const T& cr = v;
	co::Any a8;
	a8.set<const T&>( cr );
	EXPECT_TRUE( a8.getKind() == kind );
	EXPECT_TRUE( a8.isConst() );
	EXPECT_FALSE( a8.isPointer() );
	EXPECT_FALSE( a8.isPointerConst() );
	EXPECT_TRUE( a8.isReference() );
	EXPECT_EQ( sizeof(T), a8.getSize() );
	EXPECT_EQ( expectedType, a8.getType() );
	EXPECT_EQ( cr, a8.get<const T&>() );
	
	// try retrieving the reference by pointer
	EXPECT_EQ( &cr, a8.get<const T*>() );

	// test if const'ness is being enforced
	EXPECT_THROW( a8.get<T*>(), co::IllegalCastException );
}

template<typename T>
void constructorTest( const co::TypeKind kind, const T& sampleValue, co::IType* expectedType )
{
	constructorValueTest( kind, sampleValue, expectedType );
	constructorRefTest( kind, sampleValue, expectedType );
}

TEST( AnyTests, constructDefault )
{
	co::Any defaultAny;
	EXPECT_TRUE( defaultAny.getKind() == co::TK_NONE );
	EXPECT_EQ( NULL, defaultAny.getType() );
	EXPECT_ANY_TYPE_STREQ( defaultAny, "<NONE>" );

	co::Any anotherDefaultAny;
	EXPECT_TRUE( defaultAny == anotherDefaultAny );

	co::Any notDefaultAny( 0 );
	EXPECT_TRUE( defaultAny != notDefaultAny );
}

TEST( AnyTests, constructAny )
{
	co::Any anAny( 3 );
	constructorRefTest<co::Any>( co::TK_ANY, anAny, NULL );
}

TEST( AnyTests, constructBool )
{
	constructorTest<bool>( co::TK_BOOLEAN, false, NULL );
}

TEST( AnyTests, constructInt8 )
{
	constructorTest<co::int8>( co::TK_INT8, 0, NULL );
}

TEST( AnyTests, constructUInt8 )
{
	constructorTest<co::uint8>( co::TK_UINT8, 0, NULL );
}

TEST( AnyTests, constructInt16 )
{
	constructorTest<co::int16>( co::TK_INT16, 0, NULL );
}

TEST( AnyTests, constructUInt16 )
{
	constructorTest<co::uint16>( co::TK_UINT16, 0, NULL );
}

TEST( AnyTests, constructInt32 )
{
	constructorTest<co::int32>( co::TK_INT32, 0, NULL );
}

TEST( AnyTests, constructUInt32 )
{
	constructorTest<co::uint32>( co::TK_UINT32, 0, NULL );
}

TEST( AnyTests, constructInt64 )
{
	constructorTest<co::int64>( co::TK_INT64, 0, NULL );
}

TEST( AnyTests, constructUInt64 )
{
	constructorTest<co::uint64>( co::TK_UINT64, 0, NULL );
}

TEST( AnyTests, constructFloat )
{
	constructorTest<float>( co::TK_FLOAT, 0.0f, NULL );
}

TEST( AnyTests, constructDouble )
{
	constructorTest<double>( co::TK_DOUBLE, 0.0, NULL );
}

TEST( AnyTests, constructString )
{
	std::string str;
	constructorRefTest<std::string>( co::TK_STRING, str, NULL );
}

TEST( AnyTests, constructArray )
{
	co::Range<const co::int16*> r1;
	co::Any a1( r1 );
	EXPECT_EQ( sizeof(void*), a1.getSize() );
	EXPECT_ANY_TYPE_STREQ( a1, "co::Range<const co::int16*>" );

	co::Range<std::string* const> r2;
	co::Any a2( r2 );
	EXPECT_EQ( sizeof(void*), a2.getSize() );
	EXPECT_ANY_TYPE_STREQ( a2, "co::Range<std::string* const>" );

	co::Range<const std::string> r3 = co::getPaths();
	co::Any a3( r3 );
	EXPECT_EQ( sizeof(std::string), a3.getSize() );
	EXPECT_ANY_TYPE_STREQ( a3, "co::Range<const std::string>" );

	ASSERT_TRUE( r3.getSize() > 0 );

	co::Range<const std::string> r3ReadBack = a3.get<co::Range<const std::string> >();
	ASSERT_EQ( r3.getSize(), r3ReadBack.getSize() );
	while( r3 )
	{
		EXPECT_EQ( r3.getFirst(), r3ReadBack.getFirst() );
		r3.popFirst();
		r3ReadBack.popFirst();
	}

	co::RefVector<co::IInterface> r4;
	co::Any a4;
	a4.set<co::RefVector<co::IInterface>&>( r4 );
	EXPECT_EQ( sizeof(void*), a4.getSize() );
	EXPECT_ANY_TYPE_STREQ( a4, "co::RefVector<co::IInterface>" );

	EXPECT_TRUE( co::TK_NATIVECLASS == co::kindOf<co::Uuid>::kind );
	EXPECT_EQ( "co.Uuid", co::typeOf<co::Uuid>::get()->getFullName() );
	EXPECT_STREQ( "co.Uuid", co::nameOf<co::Uuid>::get() );

	std::vector<co::Uuid> r5;
	co::Any a5;
	a5.set<std::vector<co::Uuid>&>( r5 );
	EXPECT_EQ( sizeof(co::Uuid), a5.getSize() );
	EXPECT_ANY_TYPE_STREQ( a5, "std::vector<co::Uuid>" );

	std::vector<const double*> r6;
	co::Any a6;
	a6.set<std::vector<const double*>&>( r6 );
	EXPECT_EQ( sizeof(void*), a1.getSize() );
	EXPECT_ANY_TYPE_STREQ( a6, "std::vector<const double*>" );

	double d = 7.5;
	EXPECT_EQ( 0, a6.get<std::vector<const double*>&>().size() );
	a6.get<std::vector<const double*>&>().push_back( &d );
	ASSERT_EQ( 1, r6.size() );
	EXPECT_EQ( &d, r6[0] );
}

TEST( AnyTests, constructEnum )
{
	constructorTest<co::TypeKind>( co::TK_ENUM, co::TK_COMPONENT, co::typeOf<co::TypeKind>::get() );
}

std::ostream& operator<<( std::ostream& out, const co::CSLError& v )
{
	return out << "CSLError{ filename = '" << v.filename << "', msg = '" << v.message << "', line = " << v.line << "}";
}

TEST( AnyTests, constructStruct )
{
	co::CSLError cslError;
	cslError.filename = "filename";
	cslError.message = "message";
	cslError.line = 7;
	constructorRefTest<co::CSLError>( co::TK_STRUCT, cslError, co::typeOf<co::CSLError>::get() );
}

TEST( AnyTests, constructNativeClass )
{
	constructorRefTest<co::Uuid>( co::TK_NATIVECLASS, co::Uuid::createRandom(), co::typeOf<co::Uuid>::get() );
}

TEST( AnyTests, constructInterface )
{
	constructorPtrTest<co::INamespace>( co::TK_INTERFACE, co::getSystem()->getTypes()->getRootNS(),
											co::typeOf<co::INamespace>::get() );
}

/******************************************************************************
 *	A 'set and get' test checks whether a value can be correctly stored and
 *	retrieved into/from a co::Any. It may also include basic coercion tests.
 ******************************************************************************/

template<typename T>
void setAndGetTest( T correctValue, T wrongValue )
{
	T input( correctValue );
	co::Any a;
	a.set<T>( input );
	T output( a.get<T>() );
	EXPECT_EQ( input, output );
	EXPECT_EQ( output, correctValue );
	EXPECT_NE( output, wrongValue );
}

TEST( AnyTests, setGetAny )
{
	co::Any a1( 1 ), a2( 2 );
	setAndGetTest<co::Any&>( a1, a2 );
}

TEST( AnyTests, setGetBool )
{
	setAndGetTest<bool>( true, false );
	setAndGetTest<bool>( false, true );
}

TEST( AnyTests, setGetInt8 )
{
	setAndGetTest<co::int8>( 127, 0 );
	setAndGetTest<co::int8>( -128, 1 );
}

TEST( AnyTests, setGetUInt8 )
{
	setAndGetTest<co::uint8>( 0, 3 );
	setAndGetTest<co::uint8>( 255, 0 );
}

TEST( AnyTests, setGetInt16 )
{
	setAndGetTest<co::int16>( -1234, -1 );
	setAndGetTest<co::int16>( 5678, 7 );
}

TEST( AnyTests, setGetUInt16 )
{
	setAndGetTest<co::uint16>( 0, 1 );
	setAndGetTest<co::uint16>( 60000, 60001 );
}

TEST( AnyTests, setGetInt32 )
{
	setAndGetTest<co::int32>( co::MIN_INT32, co::MIN_INT32 + 1 );
	setAndGetTest<co::int32>( co::MAX_INT32, co::MAX_INT32 - 1 );
}

TEST( AnyTests, setGetUInt32 )
{
	setAndGetTest<co::uint32>( 0, -1 );
	setAndGetTest<co::uint32>( co::MAX_UINT32, co::MAX_UINT32 - 1 );
}

TEST( AnyTests, setGetInt64 )
{
	setAndGetTest<co::int64>( co::MIN_INT64, co::MIN_INT64 + 1 );
	setAndGetTest<co::int64>( co::MAX_INT64, co::MAX_INT64 - 1 );
}

TEST( AnyTests, setGetUInt64 )
{
	setAndGetTest<co::uint64>( 0, 1 );
	setAndGetTest<co::uint64>( co::MAX_UINT64, co::MAX_UINT64 - 1 );
}

TEST( AnyTests, setGetFloat )
{
	setAndGetTest<float>( -1.0f, -1.001f );
	setAndGetTest<float>( 3.14159265f, 3.14159260f );
}

TEST( AnyTests, setGetDouble )
{
	setAndGetTest<double>( -1.0, -1.00000000001 );
	setAndGetTest<double>( 3.14159265, 3.141592651 );
}

TEST( AnyTests, setGetString )
{
	std::string right( "right" );
	std::string wrong( "wrong" );
	std::string something( "something" );
	std::string somethinG( "somethinG" );

	setAndGetTest<const std::string&>( right, wrong );
	setAndGetTest<std::string&>( something, somethinG );
}

TEST( AnyTests, setGetArrays )
{
	// --- co::Range ---
	co::Range<co::IType* const> range = co::getSystem()->getTypes()->getRootNS()->getTypes();
	co::Any a1( range );
	EXPECT_ANY_TYPE_STREQ( a1, "co::Range<co::IType* const>" );
	
	// cannot drop 'const' from the array element type
	EXPECT_THROW( a1.get<co::Range<co::IType*> >(), co::IllegalCastException );

	co::Any a1Copy = a1;
	co::Range<co::IType* const> retrievedRange = a1Copy.get<co::Range<co::IType* const> >();
	EXPECT_EQ( range.getSize(), retrievedRange.getSize() );
	EXPECT_EQ( range.getFirst(), retrievedRange.getFirst() );
	EXPECT_EQ( range.getLast(), retrievedRange.getLast() );

	// --- co::RefVector ---
	co::RefVector<co::IType> refVector;
	refVector.push_back( co::typeOf<co::IObject>::get() );
	refVector.push_back( co::typeOf<co::IModule>::get() );
	refVector.push_back( co::typeOf<co::INamespace>::get() );

	co::Any a2;
	a2.set<co::RefVector<co::IType>&>( refVector );
	EXPECT_ANY_TYPE_STREQ( a2, "co::RefVector<co::IType>" );

	co::RefVector<co::IType>& refVectorRef = a2.get<co::RefVector<co::IType>&>();
	ASSERT_EQ( &refVector, &refVectorRef );

	EXPECT_EQ( 3, refVectorRef.size() );

	refVectorRef.push_back( co::typeOf<co::ITypeManager>::get() );

	EXPECT_EQ( 4, refVector.size() );
	EXPECT_EQ( "co.ITypeManager", refVector[3]->getFullName() );

	// --- std::vector ---
	std::vector<int> intVec;
	intVec.push_back( -1 );
	intVec.push_back( 5 );
	intVec.push_back( 7 );

	co::Any a3;
	a3.set<std::vector<int>&>( intVec );
	EXPECT_ANY_TYPE_STREQ( a3, "std::vector<co::int32>" );

	std::vector<int>& intVecRef = a3.get<std::vector<int>&>();

	ASSERT_EQ( &intVec, &intVecRef );
	EXPECT_EQ( 3, intVecRef.size() );

	intVecRef.push_back( 33 );

	EXPECT_EQ( 4, intVec.size() );
	EXPECT_EQ( 33, intVec[3] );
}

TEST( AnyTests, setGetEnum )
{
	setAndGetTest<co::TypeKind>( co::TK_ANY, co::TK_ARRAY );
}

TEST( AnyTests, setGetStruct )
{
	co::CSLError cslError1;
	cslError1.filename = "filename1";
	cslError1.message = "message1";
	cslError1.line = 5;

	co::CSLError cslError2;
	cslError2.filename = "filename2";
	cslError2.message = "message2";
	cslError2.line = 9;

	setAndGetTest<co::CSLError&>( cslError1, cslError2 );
}

TEST( AnyTests, setGetNativeClass )
{
	co::Uuid nullUuid = co::Uuid::null();
	co::Uuid randUuid = co::Uuid::createRandom();
	setAndGetTest<co::Uuid&>( randUuid, nullUuid );
}

bool areSamePtr( void* a, void* b )
{
	return a == b;
}

TEST( AnyTests, setGetInterface )
{
	co::Any any;

	// set 'any' with a co::IInterface instance
	co::IInterface* input = co::typeOf<co::INamespace>::get();
	any.set( input );
	EXPECT_ANY_TYPE_STREQ( any, "co::IInterface*" );

	// try to get the same co::IInterface back (exact retrieval)
	{
		co::IInterface* output = any.get<co::IInterface*>();
		EXPECT_TRUE( areSamePtr( input, output ) );
		EXPECT_EQ( input->getInterfaceOwner(), output->getInterfaceOwner() );
		EXPECT_TRUE( output->isSubTypeOf( co::typeOf<co::IService>::get() ) );
	}

	// try to get a co::IService back (implicit upcasting)
	{
		co::IService* output = any.get<co::IService*>();
		EXPECT_TRUE( areSamePtr( input, output ) );
		EXPECT_EQ( input->getInterfaceOwner(), output->getInterfaceOwner() );
	}
	
	// try to get a co::ICompositeType back (implicit upcasting)
	{
		co::ICompositeType* output = any.get<co::ICompositeType*>();
		EXPECT_TRUE( areSamePtr( input, output ) );
		EXPECT_EQ( input->getInterfaceOwner(), output->getInterfaceOwner() );
		co::IMember* res = output->getMember( "name" );
		EXPECT_EQ( res->getName(), "name" );
	}

	// try to retrieve an invalid interface
	EXPECT_THROW( any.get<co::INamespace*>(), co::IllegalCastException );

	// try to retrieve a value
	EXPECT_THROW( any.get<co::uint32>(), co::IllegalCastException );

	// set 'any' with the co::ICompositeType facet of the co::IInterface instance
	co::ICompositeType* input2 = input;
	any.set( input2 );
	EXPECT_ANY_TYPE_STREQ( any, "co::IInterface*" );

	// try to get a co::IService back (implicit upcasting)
	{
		co::IService* output = any.get<co::IService*>();
		EXPECT_TRUE( areSamePtr( input2, output ) );
		EXPECT_EQ( input2->getInterfaceOwner(), output->getInterfaceOwner() );
	}

	// try to retrieve a co::IInterface (implicit downcasting)
	{
		co::IInterface* output = any.get<co::IInterface*>();
		EXPECT_TRUE( areSamePtr( input2, output ) );
	}
}

/*****************************************************************************
 *	A 'coercion' test checks whether a stored variable is correctly
 *	implicitly converted to a different type at retrieval time.
 *****************************************************************************/

TEST( AnyTests, coercionsFromBool )
{
	co::Any a1( false );
	co::Any a2( true );

	EXPECT_ANY_STREQ( a1, "(bool)false" );
	EXPECT_ANY_STREQ( a2, "(bool)true" );

	// to int16
	EXPECT_EQ( 0, a1.get<co::int16>() );
	EXPECT_EQ( 1, a2.get<co::int16>() );

	// to double
	EXPECT_EQ( 0.0, a1.get<double>() );
	EXPECT_EQ( 1.0, a2.get<double>() );

	// to enum (co::TypeKind)
	EXPECT_EQ( co::TK_NONE, a1.get<co::TypeKind>() );
	EXPECT_EQ( co::TK_ANY, a2.get<co::TypeKind>() );

	// no conversion to string or 'reference-based' types
	EXPECT_THROW( a1.get<std::string&>(), co::IllegalCastException );
	EXPECT_THROW( a1.get<co::Uuid&>(), co::IllegalCastException );
}

TEST( AnyTests, coercionsFromUInt64 )
{
	co::Any a0; a0.set<co::uint64>( 0 );
	co::Any a1; a1.set<co::uint64>( co::MAX_INT16 );
	co::Any a2; a2.set<co::uint64>( co::MAX_UINT64 );
	
	EXPECT_ANY_STREQ( a0, "(co::uint64)0" );
	EXPECT_ANY_STREQ( a1, "(co::uint64)32767" );
	EXPECT_ANY_STREQ( a2, "(co::uint64)18446744073709551615" );

	// to bool
	EXPECT_FALSE( a0.get<bool>() );
	EXPECT_TRUE( a1.get<bool>() );
	EXPECT_TRUE( a2.get<bool>() );

	// to int16
	EXPECT_EQ( 0, a0.get<co::int16>() );
	EXPECT_EQ( co::MAX_INT16, a1.get<co::int16>() );
	EXPECT_EQ( -1, a2.get<co::int16>() ); // truncation + 2's complement = -1

	// to double
	EXPECT_EQ( 0.0, a0.get<double>() );
	EXPECT_EQ( static_cast<double>( co::MAX_INT16 ), a1.get<double>() );
	EXPECT_EQ( static_cast<double>( co::MAX_UINT64 ), a2.get<double>() );
	
	// to enum (co::TypeKind)
	EXPECT_EQ( co::TK_NONE, a0.get<co::TypeKind>() );
	EXPECT_EQ( co::TK_COMPONENT, co::Any( 20 ).get<co::TypeKind>() );
	EXPECT_THROW( a1.get<co::TypeKind>(), co::IllegalCastException );
	EXPECT_THROW( a2.get<co::TypeKind>(), co::IllegalCastException );

	// no conversion to string or 'reference-based' types
	EXPECT_THROW( a0.get<std::string&>(), co::IllegalCastException );
	EXPECT_THROW( a0.get<co::Uuid&>(), co::IllegalCastException );
}

TEST( AnyTests, coercionsFromDouble )
{
	co::Any a0( 0.0 );
	co::Any a1( 3.141592653589 );
	co::Any a2( -9.99 );

	EXPECT_ANY_STREQ( a0, "(double)0" );
	EXPECT_ANY_STREQ( a1, "(double)3.14159" );
	EXPECT_ANY_STREQ( a2, "(double)-9.99" );

	// to bool
	EXPECT_FALSE( a0.get<bool>() );
	EXPECT_TRUE( a1.get<bool>() );
	EXPECT_TRUE( a2.get<bool>() );
	
	// to int16
	EXPECT_EQ( 0, a0.get<co::int16>() );
	EXPECT_EQ( 3, a1.get<co::int16>() );
	EXPECT_EQ( -9, a2.get<co::int16>() );

	// to double/float
	EXPECT_EQ( 3.141592653589, a1.get<double>() );
	EXPECT_EQ( 3.14159265f, a1.get<float>() );

	// to enum
	EXPECT_EQ( co::TK_NONE, a0.get<co::TypeKind>() );
	EXPECT_EQ( co::TK_INT8, a1.get<co::TypeKind>() );
	EXPECT_THROW( a2.get<co::TypeKind>(), co::IllegalCastException );

	// invalid retrieval: correct primitive, but it's a value, not a reference!
	EXPECT_THROW( a1.get<double&>(), co::IllegalCastException );

	// invalid retrieval: not a pointer/interface
	EXPECT_THROW( a1.get<co::IService*>(), co::IllegalCastException );
}

TEST( AnyTests, coercionsFromEnum )
{
	co::Any a0( co::TK_NONE );
	co::Any a1( co::TK_FLOAT );
	co::Any a2( co::TK_COMPONENT );

	EXPECT_ANY_STREQ( a0, "(co::TypeKind)0" );
	EXPECT_ANY_STREQ( a1, "(co::TypeKind)11" );
	EXPECT_ANY_STREQ( a2, "(co::TypeKind)20" );

	// to bool
	EXPECT_FALSE( a0.get<bool>() );
	EXPECT_TRUE( a1.get<bool>() );
	EXPECT_TRUE( a2.get<bool>() );	

	// to int16
	EXPECT_EQ( 0, a0.get<co::int16>() );
	EXPECT_EQ( 11, a1.get<co::int16>() );
	EXPECT_EQ( 20, a2.get<co::int16>() );

	// to float
	EXPECT_EQ( 0.0f, a0.get<float>() );
	EXPECT_EQ( 11.0f, a1.get<float>() );
	EXPECT_EQ( 20.0f, a2.get<float>() );

	// NOTE: if we had a second enum type in the core, we could
	// test direct enum-to-enum coercion

	// invalid retrievals: enums should not be retrieved by reference
	EXPECT_THROW( a0.get<co::TypeKind&>(), co::IllegalCastException );
	EXPECT_THROW( a0.get<co::TypeKind*>(), co::IllegalCastException );
}

TEST( MappingTests, coercionBetweenEnums )
{	
	co::Any a0( co::TK_NONE );
	co::Any a1( static_cast<short>( 1 ) );
	co::Any a2( co::TK_BOOLEAN );
	co::Any a3( co::TK_UINT32 );
	co::Any a4( co::ModuleState_Integrated );
	co::Any a5( co::__ModuleState__FORCE_SIZEOF_UINT32 );

	EXPECT_ANY_STREQ( a0, "(co::TypeKind)0" );
	EXPECT_ANY_STREQ( a1, "(co::int16)1" );
	EXPECT_ANY_STREQ( a2, "(co::TypeKind)2" );
	EXPECT_ANY_STREQ( a3, "(co::TypeKind)8" );
	EXPECT_ANY_STREQ( a4, "(co::ModuleState)2" );
	EXPECT_ANY_STREQ( a5, "(co::ModuleState)4294967295" );

	EXPECT_EQ( 0, a0.get<short>() );
	EXPECT_EQ( co::ModuleState_None, a0.get<co::ModuleState>() );

	EXPECT_EQ( co::TK_ANY, a1.get<co::TypeKind>() );
	EXPECT_EQ( co::ModuleState_Initialized, a1.get<co::ModuleState>() );

	EXPECT_EQ( 2, a2.get<co::int8>() );
	EXPECT_EQ( co::ModuleState_Integrated, a2.get<co::ModuleState>() );
	EXPECT_EQ( co::TK_BOOLEAN, a4.get<co::TypeKind>() );

	EXPECT_EQ( 8, a3.get<co::uint64>() );
	EXPECT_THROW( a3.get<co::ModuleState>(), co::IllegalCastException );

	EXPECT_EQ( 4294967295U, a5.get<co::uint32>() );
	EXPECT_THROW( a5.get<co::TypeKind>(), co::IllegalCastException );
	EXPECT_THROW( a5.get<co::ModuleState>(), co::IllegalCastException );
}

TEST( AnyTests, coercionsFromInterface )
{
	co::Any a0( reinterpret_cast<co::IType*>( 0 ) );
	co::Any a1( co::typeOf<co::INamespace>::get() );

	EXPECT_ANY_STREQ( a0, "(co::IType*)NULL" );
	EXPECT_ANY_TYPE_STREQ( a1, "co::IInterface*" );

	// to primitives (invalid retrievals):
	EXPECT_THROW( a0.get<bool>(), co::IllegalCastException );
	EXPECT_THROW( a1.get<co::int32>(), co::IllegalCastException );
	EXPECT_THROW( a1.get<std::string*>(), co::IllegalCastException );
	EXPECT_THROW( a0.get<co::TypeKind>(), co::IllegalCastException );
	EXPECT_THROW( a0.get<co::INamespace*>(), co::IllegalCastException );

	// valid coercions to super-types:
	EXPECT_TRUE( a0.get<co::IService*>() == NULL );
	EXPECT_TRUE( a1.get<co::IService*>() != NULL );
	EXPECT_TRUE( a1.get<co::IType*>() != NULL );
}

TEST( AnyTests, coercionsFromArrayRange )
{
	// ValueType
	co::Range<float> valueRange;
	co::Any valueRangeAny( valueRange );
	EXPECT_ANY_TYPE_STREQ( valueRangeAny, "co::Range<float>" );

	// ValueType*
	co::Range<float*> valuePtrRange;
	co::Any valuePtrRangeAny( valuePtrRange );
	EXPECT_ANY_TYPE_STREQ( valuePtrRangeAny, "co::Range<float*>" );

	// const ValueType
	co::Range<const float> constValueRange;
	co::Any constValueRangeAny( constValueRange );
	EXPECT_ANY_TYPE_STREQ( constValueRangeAny, "co::Range<const float>" );
	
	// const ValueType* const
	co::Range<const float* const> constValuePtrConst;
	co::Any constValuePtrConstRangeAny( constValuePtrConst );
	EXPECT_ANY_TYPE_STREQ( constValuePtrConstRangeAny, "co::Range<const float* const>" );

	// SubInterface*
	co::Range<co::IInterface*> interfaceRange;
	co::Any interfaceRangeAny( interfaceRange );
	EXPECT_ANY_TYPE_STREQ( interfaceRangeAny, "co::Range<co::IInterface*>" );

	// SubInterface* const
	co::Range<co::IInterface* const> constInterfaceRange;
	co::Any constInterfaceRangeAny( constInterfaceRange );
	EXPECT_ANY_TYPE_STREQ( constInterfaceRangeAny, "co::Range<co::IInterface* const>" );
	
	// SuperInterface*
	co::Range<co::IType*> superInterfaceRange;
	co::Any superInterfaceRangeAny( superInterfaceRange );
	EXPECT_ANY_TYPE_STREQ( superInterfaceRangeAny, "co::Range<co::IType*>" );

	// ValueType -> const ValueType
	EXPECT_TRUE( valueRangeAny.get<co::Range<const float> >().isEmpty() );

	// ILLEGAL: const ValueType -> ValueType
	EXPECT_THROW( constValueRangeAny.get<co::Range<float> >(), co::IllegalCastException );
	
	// ILLEGAL: ValueType -> DifferentValueType
	EXPECT_THROW( valueRangeAny.get<co::Range<double> >(), co::IllegalCastException );

	// ValueType* -> const ValueType*
	EXPECT_TRUE( valuePtrRangeAny.get<co::Range<const float*> >().isEmpty() );

	// ValueType* -> ValueType* const
	EXPECT_TRUE( valuePtrRangeAny.get<co::Range<float* const> >().isEmpty() );

	// ValueType* -> const ValueType* const
	EXPECT_TRUE( valuePtrRangeAny.get<co::Range<const float* const> >().isEmpty() );

	// ILLEGAL: ValueType* -> const ValueType
	EXPECT_THROW( valuePtrRangeAny.get<co::Range<const float> >(), co::IllegalCastException );

	// ILLEGAL: const ValueType* const -> ValueType* const
	EXPECT_THROW( constValuePtrConstRangeAny.get<co::Range<float* const> >(), co::IllegalCastException );

	// ILLEGAL: const ValueType* const -> const ValueType*
	EXPECT_THROW( constValuePtrConstRangeAny.get<co::Range<const float*> >(), co::IllegalCastException );

	// SubInterface* -> SubInterface* const
	EXPECT_TRUE( interfaceRangeAny.get<co::Range<co::IInterface* const> >().isEmpty() );

	// ILLEGAL: SubInterface* const -> SubInterface*
	EXPECT_THROW( constInterfaceRangeAny.get<co::Range<co::IInterface*> >(), co::IllegalCastException );

	// ILLEGAL: SubInterface* -> SuperInterface*
	EXPECT_THROW( interfaceRangeAny.get<co::Range<co::IType*> >(), co::IllegalCastException );

	// SubInterface* -> SuperInterface* const
	EXPECT_TRUE( interfaceRangeAny.get<co::Range<co::IType* const> >().isEmpty() );

	// SubInterface* const -> SuperInterface* const
	EXPECT_TRUE( constInterfaceRangeAny.get<co::Range<co::IType* const> >().isEmpty() );

	// ILLEGAL: SuperInterface* -> SubInterface*
	EXPECT_THROW( superInterfaceRangeAny.get<co::Range<co::IInterface*> >(), co::IllegalCastException );
}

TEST( AnyTests, coercionsFromRefVector )
{
	// SubInterface*
	co::RefVector<co::IInterface> refVector;
	refVector.push_back( co::typeOf<co::IModule>::get() );
	refVector.push_back( co::typeOf<co::INamespace>::get() );

	co::Any refVectorAny;
	refVectorAny.set<co::RefVector<co::IInterface>&>( refVector );
	EXPECT_ANY_TYPE_STREQ( refVectorAny, "co::RefVector<co::IInterface>" );

	// co::RefVector<SubInterface> -> co::Range<SubInterface* const>
	EXPECT_EQ( 2, refVectorAny.get<co::Range<co::IInterface* const> >().getSize() );

	// ILLEGAL: co::RefVector<SubInterface> -> co::Range<SubInterface*>
	EXPECT_THROW( refVectorAny.get<co::Range<co::IInterface*> >(), co::IllegalCastException );

	// co::RefVector<SubInterface> -> co::Range<SuperInterface* const>
	EXPECT_EQ( 2, refVectorAny.get<co::Range<co::IType* const> >().getSize() );

	co::Range<co::IType* const> r = refVectorAny.get<co::Range<co::IType* const> >();
	EXPECT_EQ( "co.IModule", r.getFirst()->getFullName() );
	EXPECT_EQ( "co.INamespace", r.getLast()->getFullName() );
}

TEST( AnyTests, coercionsFromStdVector )
{
	// ValueType
	std::vector<int> intVec;
	intVec.push_back( -1 );
	intVec.push_back( 5 );
	intVec.push_back( 7 );

	co::Any intVecAny;
	intVecAny.set<std::vector<int>&>( intVec );
	EXPECT_ANY_TYPE_STREQ( intVecAny, "std::vector<co::int32>" );

	// const ValueType*
	std::vector<const int*> constIntPtrVec;
	co::Any constIntPtrVecAny;
	constIntPtrVecAny.set<std::vector<const int*>&>( constIntPtrVec );
	EXPECT_ANY_TYPE_STREQ( constIntPtrVecAny, "std::vector<const co::int32*>" );

	// SubInterface*
	std::vector<co::IInterface*> subVec;
	subVec.push_back( co::typeOf<co::IModule>::get() );
	subVec.push_back( co::typeOf<co::INamespace>::get() );

	co::Any subVecAny;
	subVecAny.set<std::vector<co::IInterface*>&>( subVec );
	EXPECT_ANY_TYPE_STREQ( subVecAny, "std::vector<co::IInterface*>" );

	// SuperInterface*
	std::vector<co::IType*> superVec;	
	co::Any superVecAny;
	superVecAny.set<std::vector<co::IType*>&>( superVec );
	EXPECT_ANY_TYPE_STREQ( superVecAny, "std::vector<co::IType*>" );

	// std::vector<ValueType> -> co::Range<const ValueType>
	EXPECT_EQ( 3, intVecAny.get<co::Range<const int> >().getSize() );

	// std::vector<ValueType> -> co::Range<ValueType>
	EXPECT_EQ( 3, intVecAny.get<co::Range<int> >().getSize() );

	// ILLEGAL: std::vector<ValueType> -> co::Range<DifferentValueType>
	EXPECT_THROW( intVecAny.get<co::Range<short> >(), co::IllegalCastException );

	// std::vector<const ValueType*> -> co::Range<const ValueType*>
	EXPECT_TRUE( constIntPtrVecAny.get<co::Range<const int*> >().isEmpty() );

	// std::vector<const ValueType*> -> co::Range<const ValueType* const>
	EXPECT_TRUE( constIntPtrVecAny.get<co::Range<const int* const> >().isEmpty() );

	// ILLEGAL: std::vector<const ValueType*> -> co::Range<ValueType*>
	EXPECT_THROW( constIntPtrVecAny.get<co::Range<int*> >(), co::IllegalCastException );

	// ILLEGAL: std::vector<const ValueType*> -> std::vector<ValueType*>
	EXPECT_NO_THROW( constIntPtrVecAny.get<std::vector<const int*>&>() );
	EXPECT_THROW( constIntPtrVecAny.get<std::vector<int*>&>(), co::IllegalCastException );
	
	// std::vector<SubInterface*> -> co::Range<SubInterface* const>
	EXPECT_EQ( 2, subVecAny.get<co::Range<co::IInterface* const> >().getSize() );

	// std::vector<SubInterface*> -> co::Range<SubInterface*>
	EXPECT_EQ( 2, subVecAny.get<co::Range<co::IInterface*> >().getSize() );

	// ILLEGAL: std::vector<SubInterface*> -> std::vector<SuperInterface*>
	EXPECT_EQ( 2, subVecAny.get<std::vector<co::IInterface*>&>().size() );	
	EXPECT_THROW( subVecAny.get<std::vector<co::IType*>&>(), co::IllegalCastException );

	// std::vector<SubInterface*> -> co::Range<SuperInterface* const>
	EXPECT_EQ( 2, subVecAny.get<co::Range<co::IType* const> >().getSize() );

	// ILLEGAL: std::vector<SubInterface*> -> co::Range<SuperInterface*>
	EXPECT_THROW( subVecAny.get<co::Range<co::IType*> >(), co::IllegalCastException );

	// ILLEGAL: std::vector<SuperInterface*> -> co::Range<SubInterface* const>
	EXPECT_THROW( superVecAny.get<co::Range<co::IInterface* const> >(), co::IllegalCastException );
}

/****************************************************************************
 *	Tests for the custom variable setters and constructors
 ****************************************************************************/

TEST( AnyTests, setInterface )
{
	co::IInterface* nsType = co::typeOf<co::INamespace>::get();
	co::IService* nsTypeAsItf = co::disambiguate<co::IService>( nsType );

	co::Any automatic( nsTypeAsItf );
	co::Any manual( co::disambiguate<co::IService>( nsType ), NULL );
	EXPECT_EQ( automatic, manual );

	// try passing a null pointer
	automatic.set<co::INamespace*>( NULL );
	manual.setInterface( NULL, nsType );
	EXPECT_EQ( automatic, manual );
}

TEST( AnyTests, setVariable )
{
	co::int16 anInt16 = 5;
	double aDouble = 3.14;
	std::string aString( "Hey Joe" );
	co::TypeKind anEnum = co::TK_FLOAT;

	co::Any automatic( anInt16 );
	co::Any manual( co::getType( "int16" ), co::Any::VarIsValue, &anInt16 );
	EXPECT_EQ( automatic, manual );

	automatic.set( &aDouble );
	manual.setVariable( co::getType( "double" ), co::Any::VarIsPointer, &aDouble );
	EXPECT_EQ( automatic, manual );

	automatic.set<const std::string&>( aString );
	manual.setVariable( co::getType( "string" ), co::Any::VarIsConst | co::Any::VarIsReference, &aString );
	EXPECT_EQ( automatic, manual );

	automatic.set( anEnum );
	manual.setVariable( co::getType( "co.TypeKind" ), co::Any::VarIsValue, &anEnum );
	EXPECT_EQ( automatic, manual );
}

TEST( AnyTests, setBasic )
{
	co::int16 anInt16 = 5;
	double aDouble = 3.14;
	std::string aString( "Hey Joe" );

	co::Any automatic( anInt16 );
	co::Any manual( co::TK_INT16, co::Any::VarIsValue, &anInt16 );
	EXPECT_EQ( automatic, manual );

	automatic.set( &aDouble );
	manual.setBasic( co::TK_DOUBLE, co::Any::VarIsPointer, &aDouble );
	EXPECT_EQ( automatic, manual );

	automatic.set<const std::string&>( aString );
	manual.setBasic( co::TK_STRING, co::Any::VarIsConst | co::Any::VarIsReference, &aString );
	EXPECT_EQ( automatic, manual );
}

TEST( AnyTests, setArray )
{
	std::vector<co::int8> int8Vec;
	int8Vec.push_back( -1 );
	int8Vec.push_back( 5 );
	int8Vec.push_back( 7 );

	co::RefVector<co::IInterface> refVector;
	refVector.push_back( co::typeOf<co::IModule>::get() );
	refVector.push_back( co::typeOf<co::INamespace>::get() );

	co::Range<const float* const> floatArrayRange;
	co::Range<co::int8> int8VecRange( int8Vec );

	co::Any automatic;
	automatic.set<std::vector<co::int8>&>( int8Vec );
	co::Any manual( co::Any::AK_StdVector, co::getType( "int8" ), co::Any::VarIsValue, &int8Vec );
	EXPECT_EQ( automatic, manual );

	automatic.set<co::RefVector<co::IInterface>&>( refVector );
	manual.setArray( co::Any::AK_RefVector, co::getType( "co.IInterface" ), co::Any::VarIsPointer, &refVector );
	EXPECT_EQ( automatic, manual );

	automatic.set( floatArrayRange );
	manual.setArray( co::Any::AK_ArrayRange, co::getType( "float" ), co::Any::VarIsConst|co::Any::VarIsPointerConst, NULL );
	EXPECT_EQ( automatic, manual );

	automatic.set( int8VecRange );
	manual.setArray( co::Any::AK_ArrayRange, co::getType( "int8" ), co::Any::VarIsValue, &int8Vec.front(), int8Vec.size() );
	EXPECT_EQ( automatic, manual );
}

/***************************************************************************
 *	Tests for the Temporary Objects API
 ****************************************************************************/

template<typename T>
void testTemporaryComplexValue( const T& sample )
{
	co::Any a1;
	a1.createComplexValue<T>();
	ASSERT_NE( a1.get<T&>(), sample );

	a1.get<T&>() = sample;
	ASSERT_EQ( a1.get<T&>(), sample );

	co::Any a2( a1 );
	EXPECT_EQ( a2.get<T&>(), sample );
	EXPECT_EQ( a1.get<T&>(), a2.get<T&>() );
	
	EXPECT_EQ( a1.containsObject(), true );
	EXPECT_EQ( a2.containsObject(), true );
	a1.destroyObject();

	EXPECT_EQ( a1.containsObject(), false );
	EXPECT_EQ( a2.containsObject(), true );
}

TEST( AnyTests, temporaryComplexValues )
{
	testTemporaryComplexValue<co::Uuid>( co::Uuid::createRandom() );
	
	co::CSLError cslError;
	cslError.filename = "filename";
	cslError.message = "msg";
	cslError.line = 3;
	testTemporaryComplexValue<co::CSLError>( cslError );
}
