#include "Logger.h"

#include <iostream>
#include <chrono>
#include <format>

std::string getCurrentTimeAsString();

osen::Logger::Logger(std::string sourceName)
{
	m_sourceName = sourceName;
}

namespace Colors {
	const std::string reset = "\033[0m";
	const std::string black = "\033[30m";
	const std::string red = "\033[31m";
	const std::string green = "\033[32m";
	const std::string yellow = "\033[33m";
	const std::string blue = "\033[34m";
	const std::string magenta = "\033[35m";
	const std::string cyan = "\033[36m";
	const std::string white = "\033[37m";
	const std::string gray = "\033[90m";
	const std::string brightRed = "\033[91m";
	const std::string brightGreen = "\033[92m";
	const std::string brightYellow = "\033[93m";
	const std::string brightBlue = "\033[94m";
	const std::string brightCyan = "\033[96m";
	const std::string brightWhite = "\033[97m";
}


void osen::Logger::log(LogSeverity logSeverity, std::string msg)
{
	if (logSeverity < m_minLogSeverity)
		return;

	std::string text;

	text = "[" + getCurrentTimeAsString() + "] "
		"[" + m_sourceName + "] " +
		LogSeverityToString(logSeverity) + " "
		" " + msg + "\n";

	std::cout << text;
}


std::string osen::Logger::LogSeverityToString(LogSeverity logSeverity)
{
	std::string text;

	switch (logSeverity)
	{
	case LogSeverity::TRACE:
		text += Colors::gray + "[trace]" + Colors::reset;
		break;

	case LogSeverity::DEBUG:
		text += Colors::gray + "[debug]" + Colors::reset;
		break;

	case LogSeverity::INFO:
		text += Colors::white + "[info]" + Colors::reset;
		break;

	case LogSeverity::WARNING:
		text += Colors::yellow + "[warning]" + Colors::reset;
		break;

	case LogSeverity::ERROR:
		text += Colors::red + "[error]" + Colors::reset;
		break;

	case LogSeverity::FATAL:
		text += Colors::magenta + "[fatal]" + Colors::reset;
		break;

	default:
		text += "[unknown]";
		break;
	}

	return text;
}

std::string getCurrentTimeAsString()
{
	auto now = std::chrono::system_clock::now();

	auto time = std::chrono::current_zone()->to_local(now);

	return std::format("{:%d-%m-%Y %H:%M:%OS}", time);
}
