#include "Application.h"

#include <OsenEngine/Core/Logging/Logger.h>

#include <iostream>
#include <chrono>
#include <thread>
#include "Logging/internal/coreLogger.h"

osen::Application::Application()
{
	m_previousFrameTime = std::chrono::steady_clock::now();
}

osen::Application::~Application()
{

}

void osen::Application::run()
{
	LOG(osen::coreLogger, osen::Logger::LogSeverity::ERROR, "engine started");

	while (m_isRunning)
	{

	}
}