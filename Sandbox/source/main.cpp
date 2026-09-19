#include <OsenEngine/Core/Application.h>
#include <OsenEngine/Core/Logging/Logger.h>

#include <iostream>

#include "GameLog.h"

class Sandbox : public osen::Application
{
public: 

private:

};

int main()
{
	Sandbox game{};

	gameLog.setMinLogSeverity(osen::Logger::LogSeverity::INFO);

	gameLog.log(osen::Logger::LogSeverity::TRACE, " TRACE");
	gameLog.log(osen::Logger::LogSeverity::DEBUG, " DEBUG");
	gameLog.log(osen::Logger::LogSeverity::INFO, " INFO");
	gameLog.log(osen::Logger::LogSeverity::WARNING, " WARNING");
	gameLog.log(osen::Logger::LogSeverity::ERROR, " ERROR");
	gameLog.log(osen::Logger::LogSeverity::FATAL, "FATAL");

	game.run();
}