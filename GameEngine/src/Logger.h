#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

enum LogType {
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
};

struct LogEntry {
	
	//Log, Warning, Error
	LogType type;
	std::string message;

};


class Logger
{
public:
	static std::vector<LogEntry> messages;
	static void Log(const std::string& message);
	static void Err(const std::string& messsage);
	static void Warn(const std::string& message);


};

#endif

