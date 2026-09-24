#include "Application.h"
#include <OsenEngine/Core/Logging/Logger.h>
#include <string>
#include <thread>
#include <chrono>
#include "Logging/internal/coreLogger.h"
#include "asserts.h"
#include "Window.h"

osen::Application::Application(Config config)
{
	WindowConfig windowConfig = config.windowConfig;

	m_window = Window::createWindow(windowConfig);
}

osen::Application::~Application()
{

}

void osen::Application::run()
{
	LOG(osen::coreLogger, osen::Logger::LogSeverity::INFO, "engine started");
	OSEN_ASSERT(true, "statement must be true for assert to be skipped");
	while (m_isRunning)
	{
		LOG(coreLogger, osen::Logger::LogSeverity::INFO, "updated");
		std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
	}
}