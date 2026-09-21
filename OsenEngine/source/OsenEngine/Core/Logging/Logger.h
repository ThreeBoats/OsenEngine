#pragma once

#include <iostream>


#ifdef OSEN_RELEASE

#define LOG(logger, severity, message) do {} while(false)

#else 

#define LOG(logger, severity, message) do {logger.log(severity, message);} while(false)

#endif // OSEN_RELEASE


namespace osen {
	class Logger;
}

class osen::Logger {
public:
	Logger(std::string sourceName);

	/// <summary>
	/// Defines the severity levels used by the logger.
	/// The levels go from least severe to most severe.
	/// </summary>
	enum class LogSeverity {
		TRACE,
		DEBUG,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};


	/// <summary>
	/// Logs a message if its severity is high enough.
	/// </summary>
	/// <param name="logSev">The severity of the message.</param>
	/// <param name="msg">The message to be logged.</param>
	void log(LogSeverity logSev, std::string msg);


	/// <summary>
	/// Sets the minimum severity that will be logged.
	/// Messages with a lower severity will be ignored.
	/// </summary>
	/// <param name="logSev">
	/// The minimum severity that must be met for a message to be logged.
	/// </param>
	void setMinLogSeverity(LogSeverity logSev) {
		this->log(LogSeverity::TRACE, "set mimimum log severity to: " + LogSeverityToString(logSev));
		m_minLogSeverity = logSev;
	}

private:
	/// <summary>
	/// converts the LogSeverity to string
	/// </summary>
	std::string LogSeverityToString(LogSeverity logSev);


	/// The lowest severity that will be logged.
	/// Messages with a lower severity are ignored.
	LogSeverity m_minLogSeverity = LogSeverity::TRACE;

	///where the log came from. i.e. [engine core] or [renderer], or a user defined thing
	std::string m_sourceName;
};