#include "Test.hpp"
#include "logger.h"
#include <sstream>
#include <cstdarg>
#include <string>
#include <vector>


static std::vector<std::string> g_logs;

extern void logger_format_message(LoggerLevel level, const char* filename, const char* format, ...)
{
	va_list args;
	va_start(args, format);

	std::ostringstream out;
	out << ToString(level) << " " << logger_get_filename(filename) << " ";
	while (*format != 0)
	{
		char c = *(format++);
		if (c == '%')
		{
			char f = *(format++);
			if (f == 's')
				out << va_arg(args, const char*);
			else if (f == 'd')
				out << va_arg(args, int);
			else if (f == '%')
				out.put('%');
			else
				out << "[unknown format " << f << "]";
		}
		else
			out.put(c);
	}

	va_end(args);

	g_logs.push_back(out.str());
	std::puts(out.str().c_str());
}


TEST_CASE(logger_happy_case)
{
	g_logs.clear();

	LOG_ALWAYS("Always");
	LOG_FATAL("Fatal");
	LOG_ERROR("Error");
	LOG_WARN("Warn");
	LOG_INFO("Info");
	LOG_DEBUG("Debug");
	LOG_TRACE("Trace");

	ASSERT(g_logs.size() == 4);
	ASSERT(g_logs[0] == "ALWAYS logger_tests.cpp Always");
	ASSERT(g_logs[1] == "FATAL logger_tests.cpp Fatal");
	ASSERT(g_logs[2] == "ERROR logger_tests.cpp Error");
	ASSERT(g_logs[3] == "WARN logger_tests.cpp Warn");
}

TEST_CASE(logger_show_levels)
{
	g_logs.clear();

	logger_show_levels();

	ASSERT(g_logs.size() == 1);
	ASSERT(g_logs[0] == "ALWAYS logger.c Zone 'logger_tests.cpp' log level WARN.");
}
