// file: src/logger.h

#include "logger.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "resources.h"

typedef enum LogLevel {
	Log_Format_Debug = 0,
	Log_Format_Info = 1,
	Log_Format_Warning = 2,
	Log_Format_Error = 3,
	Log_Format_Fatal = 4
} LogLevel;

static void doLog(LogLevel pLevel, const char* fmt, va_list args) {
	// build time string
	time_t t = time(NULL);
	struct tm tm_buf;
	char timeString[40];
	gmtime_s(&tm_buf, &t);
	strftime(timeString, sizeof timeString, "%A %c", &tm_buf);

	// build passed string
	int size = vsnprintf(NULL, 0, fmt, args);
	int buf_size = 1 + size * sizeof(char);
	char* buf = malloc(buf_size);
	vsnprintf(buf, buf_size, fmt, args);

	int size2;
	char* buffer = "";

	// build log string
	switch (pLevel)
	{
		case Log_Format_Debug:
			size2 = snprintf(NULL, 0, "%s [DEBUG]: %s \n", timeString, buf);
			buffer = malloc(size2 + 1);
			snprintf(buffer, size2 + 1, "%s [DEBUG]: %s \n", timeString, buf);
			break;
		case Log_Format_Info:
			size2 = snprintf(NULL, 0, "%s [INFO]: %s \n", timeString, buf);
			buffer = malloc(size2 + 1);
			snprintf(buffer, buffer, "%s [INFO]: %s \n", timeString, buf);
			break;
		case Log_Format_Warning:
			size2 = snprintf(NULL, 0, "%s [WARN]: %s \n", timeString, buf);
			buffer = malloc(size2 + 1);
			snprintf(buffer, size2 + 1, "%s [WARN]: %s \n", timeString, buf);
			break;
		case Log_Format_Error:
			size2 = snprintf(NULL, 0, "%s [ERROR]: %s \n", timeString, buf);
			buffer = malloc(size2 + 1);
			snprintf(buffer, size2 + 1, "%s [ERROR]: %s \n", timeString, buf);
			break;
		case Log_Format_Fatal:
			size2 = snprintf(NULL, 0, "%s [FATAL]: %s \n", timeString, buf);
			buffer = malloc(size2 + 1);
			snprintf(buffer, size2 + 1, "%s [FATAL]: %s \n", timeString, buf);
			break;
	}

	// write to log
	al_append_native_text_log(log, buffer);

	// free dynamic allocated ressources
	free(buffer);
}

void Logger_Debug(char* message, ...) {
	va_list args;
	va_start(args, message); //Analysis: According to the address of the parameter fmt, get the address of the parameter behind fmt and save it in the args pointer variable.
	doLog(Log_Format_Debug, message, args);
	va_end(args); //Release: release the args pointer,
}

void Logger_Info(char* message, ...) {
	va_list args;
	va_start(args, message);
	doLog(Log_Format_Info, message, args);
	va_end(args);
}

void Logger_Warning(char* message, ...) {
	va_list args;
	va_start(args, message);
	doLog(Log_Format_Warning, message, args);
	va_end(args);
}

void Logger_Error(char* message, ...) {
	va_list args;
	va_start(args, message); 
	doLog(Log_Format_Error, message, args);
	va_end(args); 
}

void Logger_Fatal(char* message, ...) {
	va_list args;
	va_start(args, message); 
	doLog(Log_Format_Fatal, message, args);
	va_end(args); 
}
