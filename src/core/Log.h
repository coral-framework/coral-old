/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#ifndef _CO_LOG_H_
#define _CO_LOG_H_

#include <co/Platform.h>
#include <sstream>

namespace co {

/*!
	\brief Log severity levels, in increasing order of severity.
	\ingroup log
 */
enum LogLevel
{
	LOG_INFO,	 //!< A harmless observation.
	LOG_WARNING, //!< Something worth checking.
	LOG_ERROR,	 //!< Potential data loss and malfunction.
	LOG_FATAL	 //!< Default action is to abort the application.
};

/*!
	\brief Contains information about a log event. See LogEventHandler.
 */
struct LogEvent
{
	std::string message;	//!< Log message.
	LogLevel level;			//!< Severity level of this event.
	bool isDebug;			//!< Whether this is a \e debug event.
};

/*!
	\brief Signature of a LogEventHandler function. See installLogEventHandler().
	\ingroup log
 */
typedef void (*LogEventHandler)( const LogEvent& event );

/*!
	\brief Installs a log event handler and returns the previous handler.

	A log event handler is a function that reports log events. The default
	handler prints all events to \c stderr, and aborts the application in case
	of a fatal error. Only a single log event handler can be defined, and it
	should generally be installed before the framework is initialized.

	To restore the default handler, call <tt>installLogEventHandler( NULL )</tt>.

	\sa debug()
	\ingroup log
 */
CORAL_EXPORT LogEventHandler installLogEventHandler( LogEventHandler handler );

/*!
	Class responsible for recording and dispatching log events.
 */
class CORAL_EXPORT Log
{
public:
	//! Returns the minimum level needed for a log event to be reported.
	inline static LogLevel getReportingLevel() { return sm_reportingLevel; }

	//! Sets the minimum severity level needed for a log event to be reported.
	static void setReportingLevel( LogLevel level );

public:
	inline Log() {;}
	~Log();

	inline std::ostringstream& init( LogLevel level = LOG_INFO, bool isDebug = false )
	{
		_level = level;
		_isDebug = isDebug;
		return _os;
	}

private:
	// forbid copies
	Log( const Log& );
	Log& operator=( const Log& );

	static LogLevel sm_reportingLevel;

private:
	bool _isDebug;
	LogLevel _level;
	std::ostringstream _os;
};

} // namespace co

/*!
	\def CORAL_LOG_MIN_SEVERITY
	Log events below this severity level are disabled at compile time.
	Defaults to LOG_INFO.

	\ingroup log
 */
#ifndef CORAL_LOG_MIN_SEVERITY
#define CORAL_LOG_MIN_SEVERITY co::LOG_INFO
#endif

#define CORAL_LOG_LEVEL_DEBUG( level, debug ) \
	if( level < CORAL_LOG_MIN_SEVERITY ) ; \
	else if( level < co::Log::getReportingLevel() ) ; \
	else co::Log().init( level, debug )

/*!
	\def CORAL_LOG( level )
	Generates a log event with the given severity level.
	The macro returns an output stream for recording the log message.

	\sa installDebugEventHandler()
	\ingroup log
 */
#define CORAL_LOG( level ) CORAL_LOG_LEVEL_DEBUG( co::LOG_##level, false )

/*!
	\def CORAL_DLOG( level )
	Just like CORAL_LOG(), but meant for debugging.
	This macro has no effect in release builds.

	\ingroup log
 */
#ifdef CORAL_NDEBUG
	#define CORAL_DLOG( level ) if( false ) co::Log().init()
#else
	#define CORAL_DLOG( level ) CORAL_LOG_LEVEL_DEBUG( co::LOG_##level, true )
#endif

#endif // _CO_LOG_H_
