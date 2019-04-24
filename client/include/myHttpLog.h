#ifndef _HTTP_LOG_H
#define _HTTP_LOG_H

#include <cstdio>
#include <string>
#include <map>

namespace httpLog
{
	enum class logLevel
	{
		DEBUG=1,
		INFO,
		WARN,
		ERROR
	};

	const std::map<httpLog::logLevel, std::string> logLevelString =
	{
		{ logLevel::DEBUG ,  "Log_Debug" },
		{ logLevel::INFO  ,  "Log_Info"  },
		{ logLevel::WARN  ,  "Log_Warn"  },
		{ logLevel::ERROR ,  "Log_Error" }
	};
}

#define Http_Log_Level 2

#define HttpLog(fd, logLevel, format, ...) \
do \
{ \
	if(static_cast<int>(logLevel) >= Http_Log_Level) \
	{ \
		fprintf(fd, "[%s ： %s @ line %d]" format "\n", \
					httpLog::logLevelString.at(logLevel).c_str(), \
					__func__, \
					__LINE__, \
					##__VA_ARGS__); \
	} \
}while(0)

#define HTTP_LOG_DEBUG(format, ...)   HttpLog(stdout, httpLog::logLevel::DEBUG, format, ##__VA_ARGS__)
#define HTTP_LOG_INFO(format, ...)    HttpLog(stdout, httpLog::logLevel::INFO , format, ##__VA_ARGS__)
#define HTTP_LOG_WARN(format, ...)    HttpLog(stderr, httpLog::logLevel::WARN , format, ##__VA_ARGS__)
#define HTTP_LOG_ERROR(format, ...)   HttpLog(stderr, httpLog::logLevel::ERROR, format, ##__VA_ARGS__)

#endif