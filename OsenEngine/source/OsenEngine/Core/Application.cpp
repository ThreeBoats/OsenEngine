#include "Application.h"

#include <OsenEngine/Core/Logging/Logger.h>
#include "Logging/internal/coreLogger.h"
#include "asserts.h"
#include "Window.h"

#include <thread>
#include <chrono>
#include <filesystem>

#include <vulkan/vulkan.hpp>

osen::Application::Application(EngineConfig config)
{
	LOG(coreLogger, Logger::LogSeverity::INFO, "engine started");
	m_window = Window::create(config);
	m_renderer = Renderer::create(config, *m_window);
}

osen::Application::~Application()
{

}

void osen::Application::run()
{
	LOG(coreLogger, Logger::LogSeverity::INFO, "engine ready");
	LOG(coreLogger, Logger::LogSeverity::INFO,
		"working directory: " + std::filesystem::current_path().string());

	while (m_isRunning)
	{
		m_window->processEvents();
		m_renderer->tmpDraw();
		std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
	}
}