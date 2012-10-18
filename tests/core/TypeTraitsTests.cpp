/*
 * Coral - Lightweight C++ Component Framework
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

TEST( TypeTraitsTests, isSubTypeOf )
{
	EXPECT_TRUE(( isSubTypeOf<int, int>::value ));
	EXPECT_TRUE(( isSubTypeOf<const int, int>::value ));
	EXPECT_TRUE(( isSubTypeOf<int, const int>::value ));
	EXPECT_FALSE(( isSubTypeOf<short, int>::value ));
	EXPECT_FALSE(( isSubTypeOf<int*, int>::value ));

	EXPECT_TRUE(( isSubTypeOf<co::IInterface, co::IClassType>::value ));
	EXPECT_TRUE(( isSubTypeOf<co::IInterface const, co::IClassType>::value ));
	EXPECT_TRUE(( isSubTypeOf<co::IInterface, co::IClassType const>::value ));
	EXPECT_FALSE(( isSubTypeOf<co::IClassType, co::IInterface>::value ));
}

TEST( TypeTraitsTests, kindOf )
{
	EXPECT_TRUE( co::kindOf<std::exception>::kind == co::TK_NULL );

	EXPECT_TRUE( co::kindOf<bool>::kind == co::TK_BOOL );

	EXPECT_TRUE( co::kindOf<int>::kind == co::TK_INT32 );

	EXPECT_TRUE( co::kindOf<co::uint8>::kind == co::TK_UINT8 );
	EXPECT_TRUE( co::kindOf<float>::kind == co::TK_FLOAT );
	EXPECT_TRUE( co::kindOf<double>::kind == co::TK_DOUBLE );

	EXPECT_TRUE( co::kindOf<std::string>::kind == co::TK_STRING );
	EXPECT_TRUE( co::kindOf<const char*>::kind == co::TK_NULL );
	
	EXPECT_TRUE( co::kindOf<co::TypeKind>::kind == co::TK_ENUM );

	EXPECT_TRUE( co::kindOf<co::Exception>::kind == co::TK_EXCEPTION );

	EXPECT_TRUE( co::kindOf<co::Range<float> >::kind == co::TK_ARRAY );
	EXPECT_TRUE( co::kindOf<co::Range<std::string> >::kind == co::TK_ARRAY );
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

	EXPECT_STREQ( "null", co::nameOf<std::exception>::get() );

	EXPECT_STREQ( "float[]", co::nameOf<co::Range<float> >::get() );
	EXPECT_STREQ( "string[]", co::nameOf<co::Range<std::string> >::get() );
	EXPECT_STREQ( "co.IInterface[]", co::nameOf<co::RefVector<co::IInterface> >::get() );
	EXPECT_STREQ( "co.Uuid[]", co::nameOf<std::vector<co::Uuid> >::get() );
}

TEST( TypeTraitsTests, typeOf )
{
	EXPECT_TRUE( co::typeOf<co::int8>::get()->getKind() == co::TK_INT8 );
	EXPECT_TRUE( co::typeOf<co::uint8>::get()->getKind() == co::TK_UINT8 );

	EXPECT_TRUE( co::typeOf<float>::get()->getKind() == co::TK_FLOAT );
	EXPECT_TRUE( co::typeOf<double>::get()->getKind() == co::TK_DOUBLE );

	EXPECT_TRUE( co::typeOf<std::string>::get()->getKind() == co::TK_STRING );

	EXPECT_EQ( co::TK_FLOAT, co::typeOf<co::Range<float> >::get()->getElementType()->getKind() );
	EXPECT_EQ( co::TK_STRING, co::typeOf<co::Range<std::string> >::get()->getElementType()->getKind() );

	co::IArray* at;
	ASSERT_TRUE( NULL != ( at = co::typeOf<co::RefVector<co::IInterface> >::get() ) );
	EXPECT_EQ( "co.IInterface", at->getElementType()->getFullName() );

	ASSERT_TRUE( NULL != ( at = co::typeOf<std::vector<co::Uuid> >::get() ) );
	EXPECT_EQ( "co.Uuid", at->getElementType()->getFullName() );
}
