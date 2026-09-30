#pragma once

#include <memory>
#include <chrono>

#include "Window.h"
#include <OsenEngine/Rendering/Renderer.h>

namespace osen {
	struct Config
	{
		WindowConfig windowConfig;
	};

	class Application
	{
	public:
		Application(Config config);
		virtual ~Application();
		void run();
	protected:

	private:
		bool m_isRunning = true;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
	};

}