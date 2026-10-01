#include <OsenEngine/Core/Application.h>
#include <OsenEngine/Core/Logging/Logger.h>
#include "GameLog.h"

#include <iostream>

#include <OsenEngine/Core/Config.h>

class Sandbox : public osen::Application
{
public:
	Sandbox(osen::EngineConfig config)
		:Application(config)
	{
	}
private:

};

int main()
{
	osen::WindowConfig windowConfig;
	windowConfig.height = 600;
	windowConfig.width = 1200;
	windowConfig.name = "CoolGame.exe (not a virus)";

	osen::RendererConfig rendererConfig;
	rendererConfig.api = osen::GraphicsAPI::VULKAN;

	osen::EngineConfig config;

	config.windowConfig = windowConfig;
	config.rendererConfig = rendererConfig;

	Sandbox game{ config };

	LOG(gameLog, osen::Logger::LogSeverity::WARNING, "dit is gedaan via macro. wauwie");

	game.run();
}