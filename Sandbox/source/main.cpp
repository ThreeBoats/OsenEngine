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

	LOG(gameLog, osen::Logger::LogSeverity::WARNING, "dit is gedaan via macro. wauwie");

	game.run();
}