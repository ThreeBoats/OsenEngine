#include <OsenEngine/Core/Application.h>
#include <OsenEngine/Core/Logging/Logger.h>

#include <iostream>

#include "GameLog.h"

class Sandbox : public osen::Application
{
public: 
	Sandbox(osen::Config config)
		:Application(config)
	{}
private:

};

int main()
{
	osen::WindowConfig windowConfig;
	windowConfig.height = 400;
	windowConfig.width= 400;
	windowConfig.name = "Sandbox";

	osen::Config config;

	config.windowConfig = windowConfig;

	Sandbox game{config};

	LOG(gameLog, osen::Logger::LogSeverity::WARNING, "dit is gedaan via macro. wauwie");

	game.run();
}