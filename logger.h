// file: src/logger.h
#pragma once

/*
* Log Debug messages.
* @Param message String to log with format providers
*/
void Logger_Debug(char* message, ...);

/*
* Log Debug messages.
* @Param message String to log with format providers
*/
void Logger_Info(char* message, ...);

/*
* Log Debug messages.
* @Param message String to log with format providers
*/
void Logger_Warning(char* message, ...);

/*
* Log Debug messages.
* @Param message String to log with format providers
*/
void Logger_Error(char* message, ...);

/*
* Log Debug messages.
* @Param message String to log with format providers
*/
void Logger_Fatal(char* message, ...);
