#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <type_traits>
#include <iostream>
#include <cctype>
#include <string>
#include <time.h>
#include <Windows.h>

#define Logger_WriteConsole(message, level) __WriteConsole(message, std::string(__FUNCTION__), level)
#define Logger_ReadConsole() __ReadConsole()
#define Logger_ThrowError(unexpectedd, message, fatal) __ThrowError(unexpectedd, message, __LINE__, std::string(__FUNCTION__), fatal)

enum class LogLevel
{
	INFO,
	WARNING,
	ISSUE,
	FATAL_ERROR
};

void __WriteConsole(const std::string& message, const std::string& function, LogLevel level)
{
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);

	std::string name;
	int position = 0;

	while (function[position] != ':' && position < function.size())
	{
		name += function[position];

		position += 1;
	}

	name[0] = std::toupper(name[0]);

	switch (level)
	{

	case LogLevel::INFO:
		printf("\033[1;32m");
		printf("[%d:%d:%d] [Thread/INFO] [%s] %s\n", tm->tm_hour, tm->tm_sec, tm->tm_min, name.c_str(), message.c_str());
		printf("\033[0m");
		break;

	case LogLevel::WARNING:
		printf("\033[1;33m");
		printf("[%d:%d:%d] [Thread/WARNING] [%s] %s\n", tm->tm_hour, tm->tm_sec, tm->tm_min, name.c_str(), message.c_str());
		printf("\033[0m");
		break;

	case LogLevel::ISSUE:
		printf("\033[1;31m");
		printf("[%d:%d:%d] [Thread/ERROR] [%s] %s\n", tm->tm_hour, tm->tm_sec, tm->tm_min, name.c_str(), message.c_str());
		printf("\033[0m");
		break;

	case LogLevel::FATAL_ERROR:
		printf("\033[1;31m");
		printf("[%d:%d:%d] [Thread/FATAL ERROR] [%s] %s\n", tm->tm_hour, tm->tm_sec, tm->tm_min, name.c_str(), message.c_str());
		printf("\033[0m");
		exit(1);
		break;

	default:
		break;

	}
}

std::string __ReadConsole()
{
	std::string out;
	std::cin >> out;
	return out;
}

void __ThrowError(const std::string& unexpected, const std::string& message, const int& line, const std::string& function, const bool& fatal)
{
	std::string name;
	int position = 0;

	while (function[position] != ':' && position < function.size())
	{
		name += function[position];

		position += 1;
	}

	name[0] = std::toupper(name[0]);

	if (!fatal)
		__WriteConsole("Unexpected >>" + unexpected + "<<, " + message + ", at: '" + function + "::" + std::to_string(line) + "'.", name, LogLevel::ISSUE);
	else
	{
		MessageBoxA(NULL, message.c_str(), unexpected.c_str(), MB_ICONERROR);
		__WriteConsole("Unexpected >>" + unexpected + "<<, " + message + ", at: '" + function + "::" + std::to_string(line) + "'.", name, LogLevel::FATAL_ERROR);
	}
}

#endif // !LOGGER_HPP