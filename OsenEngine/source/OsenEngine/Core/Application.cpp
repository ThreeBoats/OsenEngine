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
	int i = 0;

	osen::coreLogger.log(Logger::LogSeverity::ERROR, "OsenEngine not complete yet");

	while (m_isRunning)
	{
		auto startFrameTime = std::chrono::steady_clock::now();

		auto deltaTime = std::chrono::duration<double>(
			startFrameTime - m_previousFrameTime);

		m_previousFrameTime = startFrameTime;
	}
}