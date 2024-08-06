#include "Logger.h"
#include <iostream>
#include <time.h>
#include <Windows.h>


std::vector<LogEntry> Logger::messages;


void Logger::Log(const std::string& message) 
{
	//TODO:
	//Print on the console the message
	//LOG: [ dd/mm/year time ] - Here goes the message
	//Display this in green
	

	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	//SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

	char buffer[256];
	
	const std::time_t time = std::time(0);

	std::tm currentTime;
	localtime_s(&currentTime ,&time);


	strftime(buffer, sizeof(buffer), "LOG | [ %d/%b/%Y %T ] - ", &currentTime);


	std::cout << "\033[32m" << buffer << message <<  "\033[0m " << '\n';

	LogEntry logEntry;
	logEntry.type = LOG_INFO;
	logEntry.message = buffer;
	messages.push_back(logEntry);

	
}

void Logger::Warn(const std::string& message) {
	char buffer[256];

	const std::time_t time = std::time(0);

	std::tm currentTime;
	localtime_s(&currentTime, &time);


	strftime(buffer, sizeof(buffer), "LOG | [ %d/%b/%Y %T ] - ", &currentTime);


	std::cout << "\033[33m" << buffer << message << "\033[0m " << std::endl;
	messages.push_back(LogEntry{ LOG_WARNING,message });

	LogEntry logEntry;
	logEntry.type = LOG_WARNING;
	logEntry.message = buffer;
	messages.push_back(logEntry);

}

void Logger::Err(const std::string& message) 
{
	//TODO:
	//Print on the console the message
	//ERR: [ dd/mm/year time ] - Here goes the message
	//This should be in red


	char buffer[256];

	const std::time_t time = std::time(0);

	std::tm currentTime;
	localtime_s(&currentTime, &time);


	strftime(buffer, sizeof(buffer), "ERR | [ %d/%b/%Y %T ] - ", &currentTime);


	std::cout << "\033[31m" << buffer << message << "\033[0m " << std::endl;

	messages.push_back(LogEntry{ LOG_ERROR,message });

	LogEntry logEntry;
	logEntry.type = LOG_ERROR;
	logEntry.message = buffer;
	messages.push_back(logEntry);

}