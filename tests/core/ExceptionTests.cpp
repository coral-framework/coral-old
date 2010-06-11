/*
 * Coral - A C++ Component Framework.
 * See Copyright Notice in Coral.h
 */

#include <co/Exception.h>
#include <gtest/gtest.h>

TEST( ExceptionTests, throwTest )
{
	EXPECT_THROW( throw co::Exception( "message" ), co::Exception );
}

TEST( ExceptionTests, messageTest )
{
	try
	{
		throw co::Exception( "a message" );
	}
	catch( co::Exception& e )
	{
		EXPECT_TRUE( e.getMessage() == "a message" );
		e.setMessage( "another message" );
		EXPECT_TRUE( e.getMessage() == "another message" );
		return;
	}
	EXPECT_FALSE( "Catch failed!" );
}

TEST( ExceptionTests, whatTest )
{
	std::string message = "what message";
	try
	{
		throw co::Exception( message );
	}
	catch( co::Exception& e )
	{
		EXPECT_TRUE( message == e.what() );
		return;
	}
	EXPECT_FALSE( "Catch failed!" );
}
