#pragma once

#include <chrono>
#include "Window.h"
#include <memory>

namespace osen {
	struct Config
	{
		WindowConfig windowConfig;
	};


	class Application;
}

class osen::Application
{
public:
	Application(Config config);
	virtual ~Application();
	void run();
protected:

private:
	bool m_isRunning = true;
	
	std::unique_ptr<Window> m_window;
};