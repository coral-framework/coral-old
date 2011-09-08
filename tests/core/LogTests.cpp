/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include <co/Log.h>
#include <gtest/gtest.h>

static int sg_logEventCount( 0 );
static co::LogEvent sg_lastLogEvent;

static void myLogEventHandler( const co::LogEvent& event )
{
	++sg_logEventCount;
	sg_lastLogEvent = event;
}

TEST( LogTests, customEventHandler )
{
	co::LogEventHandler originalHandler = co::installLogEventHandler( myLogEventHandler );

	EXPECT_EQ( 0, sg_logEventCount );

	CORAL_LOG(INFO) << "some info";
	EXPECT_EQ( 1, sg_logEventCount );
	EXPECT_EQ( "some info", sg_lastLogEvent.message );
	EXPECT_EQ( co::LOG_INFO, sg_lastLogEvent.level );
	EXPECT_FALSE( sg_lastLogEvent.isDebug );

	CORAL_LOG(WARNING) << "some warning";
	EXPECT_EQ( 2, sg_logEventCount );
	EXPECT_EQ( "some warning", sg_lastLogEvent.message );
	EXPECT_EQ( co::LOG_WARNING, sg_lastLogEvent.level );
	EXPECT_FALSE( sg_lastLogEvent.isDebug );

	CORAL_LOG(ERROR) << "some error";
	EXPECT_EQ( 3, sg_logEventCount );
	EXPECT_EQ( "some error", sg_lastLogEvent.message );
	EXPECT_EQ( co::LOG_ERROR, sg_lastLogEvent.level );
	EXPECT_FALSE( sg_lastLogEvent.isDebug );

	CORAL_LOG(FATAL) << "fatal error";
	EXPECT_EQ( 4, sg_logEventCount );
	EXPECT_EQ( "fatal error", sg_lastLogEvent.message );
	EXPECT_EQ( co::LOG_FATAL, sg_lastLogEvent.level );
	EXPECT_FALSE( sg_lastLogEvent.isDebug );

	CORAL_DLOG(WARNING) << "test debug warning";

#ifdef CORAL_NDEBUG
	EXPECT_EQ( 4, sg_logEventCount );	
#else
	EXPECT_EQ( 5, sg_logEventCount );
	EXPECT_EQ( "test debug warning", sg_lastLogEvent.message );
	EXPECT_EQ( co::LOG_WARNING, sg_lastLogEvent.level );
	EXPECT_TRUE( sg_lastLogEvent.isDebug );	
#endif

	co::installLogEventHandler( originalHandler );
}
