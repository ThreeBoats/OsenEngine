#include "Application.h"

#include <OsenEngine/Core/Logging/Logger.h>
#include "Logging/internal/coreLogger.h"
#include "asserts.h"
#include "Window.h"

#include <thread>
#include <chrono>

#include <vulkan/vulkan.hpp>

osen::Application::Application(Config config)
{
	WindowConfig windowConfig = config.windowConfig;

	m_window = Window::create(windowConfig);
	m_renderer = Renderer::create();
}

osen::Application::~Application()
{

}

void osen::Application::run()
{
	LOG(coreLogger, Logger::LogSeverity::INFO, "engine started");

	while (m_isRunning)
	{
		m_window->processEvents();

		std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
	}
}