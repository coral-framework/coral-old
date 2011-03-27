/*
 * Coral - A lightweight C++ component framework
 * See Copyright Notice in Coral.h
 */

#include <co/Uuid.h>
#include <co/Coral.h>
#include <co/IArray.h>
#include <co/Exception.h>
#include <co/RefVector.h>
#include <co/TypeTraits.h>
#include <co/IInterface.h>
#include <gtest/gtest.h>

using namespace co::traits;

TEST( TypeTraitsTests, isEqual )
{
	EXPECT_FALSE(( isSame<int, char>::value ));
	EXPECT_FALSE(( isSame<int, int*>::value ));
	EXPECT_FALSE(( isSame<int, int&>::value ));
	EXPECT_FALSE(( isSame<int*, int* const>::value ));
	EXPECT_TRUE(( isSame<int, int>::value ));
}

TEST( TypeTraitsTests, isConst )
{
	EXPECT_FALSE(( isConst<int>::value ));
	EXPECT_FALSE(( isConst<int*>::value ));
	EXPECT_TRUE(( isConst<const int>::value ));
	EXPECT_FALSE(( isConst<const int&>::value ));
	EXPECT_FALSE(( isConst<const int*>::value ));
	EXPECT_TRUE(( isConst<const int* const>::value ));
}

TEST( TypeTraitsTests, isPointer )
{
	EXPECT_FALSE(( isPointer<int>::value ));
	EXPECT_FALSE(( isPointer<int&>::value ));
	EXPECT_TRUE(( isPointer<int*>::value ));
	EXPECT_TRUE(( isPointer<const int*>::value ));
	EXPECT_TRUE(( isPointer<int* const>::value ));
}

TEST( TypeTraitsTests, isReference )
{
	EXPECT_FALSE(( isReference<int>::value ));
	EXPECT_FALSE(( isReference<int*>::value ));
	EXPECT_TRUE(( isReference<int&>::value ));
	EXPECT_TRUE(( isReference<const int&>::value ));
}

TEST( TypeTraitsTests, removeConst )
{
	EXPECT_FALSE(( isSame<const int, int>::value ));
	EXPECT_TRUE(( isSame<removeConst<const int>::Type, int>::value ));
	EXPECT_TRUE(( isSame<removeConst<int>::Type, int>::value ));
}

TEST( TypeTraitsTests, removePointer )
{
	EXPECT_FALSE(( isSame<int*, int>::value ));
	EXPECT_TRUE(( isSame<removePointer<int*>::Type, int>::value ));
	EXPECT_TRUE(( isSame<removePointer<int**>::Type, int*>::value ));
	EXPECT_TRUE(( isSame<removePointer<int* const>::Type, int>::value ));
	EXPECT_TRUE(( isSame<removePointer<int>::Type, int>::value ));
}

TEST( TypeTraitsTests, removeReference )
{
	EXPECT_FALSE(( isSame<int&, int>::value ));
	EXPECT_TRUE(( isSame<removeReference<int&>::Type, int>::value ));
	EXPECT_TRUE(( isSame<removeReference<int>::Type, int>::value ));
}

TEST( TypeTraitsTests, get )
{
	typedef co::traits::get<const co::int32* const &> getA;
	EXPECT_TRUE(( isSame<getA::ReferencedType, const co::int32* const>::value ));
	EXPECT_TRUE(( isSame<getA::ValueType, const co::int32*>::value ));
	EXPECT_TRUE(( isSame<getA::PointedType, const co::int32>::value ));
	EXPECT_TRUE(( isSame<getA::CoreType, co::int32>::value ));
	EXPECT_TRUE( getA::kind == co::TK_INT32 );
	EXPECT_TRUE( getA::isConst );
	EXPECT_TRUE( getA::isPointer );
	EXPECT_TRUE( getA::isPointerConst );
	EXPECT_TRUE( getA::isReference );
	
	typedef co::traits::get<bool* const> getB;
	EXPECT_TRUE(( isSame<getB::ReferencedType, bool* const>::value ));
	EXPECT_TRUE(( isSame<getB::ValueType, bool*>::value ));
	EXPECT_TRUE(( isSame<getB::PointedType, bool>::value ));
	EXPECT_TRUE(( isSame<getB::CoreType, bool>::value ));
	EXPECT_TRUE( getB::kind == co::TK_BOOLEAN );
	EXPECT_FALSE( getB::isConst );
	EXPECT_TRUE( getB::isPointer );
	EXPECT_TRUE( getB::isPointerConst );
	EXPECT_FALSE( getB::isReference );

	typedef co::traits::get<const std::string&> getC;
	EXPECT_TRUE(( isSame<getC::ReferencedType, const std::string>::value ));
	EXPECT_TRUE(( isSame<getC::ValueType, std::string>::value ));
	EXPECT_TRUE(( isSame<getC::PointedType, const std::string>::value ));
	EXPECT_TRUE(( isSame<getC::CoreType, std::string>::value ));
	EXPECT_TRUE( getC::kind == co::TK_STRING );
	EXPECT_TRUE( getC::isConst );
	EXPECT_FALSE( getC::isPointer );
	EXPECT_FALSE( getC::isPointerConst );
	EXPECT_TRUE( getC::isReference );

	typedef co::traits::get<const char*> getD;
	EXPECT_TRUE(( isSame<getD::ReferencedType, const char*>::value ));
	EXPECT_TRUE(( isSame<getD::ValueType, const char*>::value ));
	EXPECT_TRUE(( isSame<getD::PointedType, const char>::value ));
	EXPECT_TRUE(( isSame<getD::CoreType, char>::value ));
	EXPECT_TRUE( getD::kind == co::TK_INT8 );
	EXPECT_TRUE( getD::isConst );
	EXPECT_TRUE( getD::isPointer );
	EXPECT_FALSE( getD::isPointerConst );
	EXPECT_FALSE( getD::isReference );

	typedef co::traits::get<const co::Exception*> getE;
	EXPECT_TRUE(( isSame<getE::ReferencedType, const co::Exception*>::value ));
	EXPECT_TRUE(( isSame<getE::ValueType, const co::Exception*>::value ));
	EXPECT_TRUE(( isSame<getE::PointedType, const co::Exception>::value ));
	EXPECT_TRUE(( isSame<getE::CoreType, co::Exception>::value ));
	EXPECT_TRUE( getE::kind == co::TK_EXCEPTION );
	EXPECT_TRUE( getE::isConst );
	EXPECT_TRUE( getE::isPointer );
	EXPECT_FALSE( getE::isPointerConst );
	EXPECT_FALSE( getE::isReference );
}

TEST( TypeTraitsTests, kindOf )
{
	EXPECT_TRUE( co::kindOf<std::exception>::kind == co::TK_NONE );

	EXPECT_TRUE( co::kindOf<bool>::kind == co::TK_BOOLEAN );

	EXPECT_TRUE( co::kindOf<int>::kind == co::TK_INT32 );

	EXPECT_TRUE( co::kindOf<co::uint8>::kind == co::TK_UINT8 );
	EXPECT_TRUE( co::kindOf<float>::kind == co::TK_FLOAT );
	EXPECT_TRUE( co::kindOf<double>::kind == co::TK_DOUBLE );

	EXPECT_TRUE( co::kindOf<std::string>::kind == co::TK_STRING );
	EXPECT_TRUE( co::kindOf<const char*>::kind == co::TK_NONE );
	
	EXPECT_TRUE( co::kindOf<co::TypeKind>::kind == co::TK_ENUM );

	EXPECT_TRUE( co::kindOf<co::Exception>::kind == co::TK_EXCEPTION );

	EXPECT_TRUE( co::kindOf<co::Range<float> >::kind == co::TK_ARRAY );
	EXPECT_TRUE( co::kindOf<co::Range<const std::string> >::kind == co::TK_ARRAY );
	EXPECT_TRUE( co::kindOf<co::RefVector<co::IInterface> >::kind == co::TK_ARRAY );
	EXPECT_TRUE( co::kindOf<std::vector<const co::Uuid*> >::kind == co::TK_ARRAY );
}

TEST( TypeTraitsTests, nameOf )
{
	EXPECT_STREQ( "bool", co::nameOf<bool>::get() );

	EXPECT_STREQ( "int8", co::nameOf<co::int8>::get() );
	EXPECT_STREQ( "uint8", co::nameOf<co::uint8>::get() );

	EXPECT_STREQ( "float", co::nameOf<float>::get() );
	EXPECT_STREQ( "double", co::nameOf<double>::get() );

	EXPECT_STREQ( "string", co::nameOf<std::string>::get() );

	EXPECT_STREQ( "co.TypeKind", co::nameOf<co::TypeKind>::get() );

	EXPECT_STREQ( "<none>", co::nameOf<std::exception>::get() );

	EXPECT_STREQ( "float[]", co::nameOf<co::Range<float> >::get() );
	EXPECT_STREQ( "string[]", co::nameOf<co::Range<const std::string> >::get() );
	EXPECT_STREQ( "co.IInterface[]", co::nameOf<co::RefVector<co::IInterface> >::get() );
	EXPECT_STREQ( "co.Uuid[]", co::nameOf<std::vector<const co::Uuid*> >::get() );
}

TEST( TypeTraitsTests, typeOf )
{
	EXPECT_TRUE( co::typeOf<co::int8>::get()->getKind() == co::TK_INT8 );
	EXPECT_TRUE( co::typeOf<co::uint8>::get()->getKind() == co::TK_UINT8 );

	EXPECT_TRUE( co::typeOf<float>::get()->getKind() == co::TK_FLOAT );
	EXPECT_TRUE( co::typeOf<double>::get()->getKind() == co::TK_DOUBLE );

	EXPECT_TRUE( co::typeOf<std::string>::get()->getKind() == co::TK_STRING );

	EXPECT_EQ( co::TK_FLOAT, co::typeOf<co::Range<float> >::get()->getElementType()->getKind() );
	EXPECT_EQ( co::TK_STRING, co::typeOf<co::Range<const std::string> >::get()->getElementType()->getKind() );
	
	co::IArray* at;
	ASSERT_TRUE( NULL != ( at = co::typeOf<co::RefVector<co::IInterface> >::get() ) );
	EXPECT_EQ( "co.IInterface", at->getElementType()->getFullName() );

	ASSERT_TRUE( NULL != ( at = co::typeOf<std::vector<const co::Uuid*> >::get() ) );
	EXPECT_EQ( "co.Uuid", at->getElementType()->getFullName() );
}
