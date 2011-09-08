/*
 * Coral - Lightweight C++ Component Framework
 * See Copyright Notice in Coral.h
 */

#include "Log.h"
#include <cstdlib>
#include <iostream>

namespace co {

static void defaultLogEventHandler( const LogEvent& event )
{
	static const char* s_eventName[] = { "INFO", "WARNING", "ERROR", "FATAL" };
	
	LogLevel level = event.level;
	if( event.level < LOG_INFO || event.level > LOG_FATAL )
		level = LOG_FATAL;

	const char* eventName = s_eventName[level];
	const char* debugPrefix = event.isDebug ? "D" : "";

	std::cerr << '[' << debugPrefix << eventName << "] " << event.message << ::std::endl;

	if( event.level == LOG_FATAL )
		abort();
}

static LogEventHandler sg_logEventHandler( defaultLogEventHandler );

LogEventHandler installLogEventHandler( LogEventHandler handler )
{
	LogEventHandler previous = sg_logEventHandler;
	sg_logEventHandler = handler ? handler : defaultLogEventHandler;
	return previous;
}

LogLevel Log::sm_reportingLevel( LOG_INFO );

void Log::setReportingLevel( LogLevel level )
{
	sm_reportingLevel = level;
}

Log::~Log()
{
	LogEvent event;
	event.message = _os.str();
	event.level = _level;
	event.isDebug = _isDebug;
	sg_logEventHandler( event );
}

} // namespace co
