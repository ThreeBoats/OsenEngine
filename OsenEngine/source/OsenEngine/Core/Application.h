#pragma once

#include <memory>
#include <chrono>

#include "Window.h"
#include <OsenEngine/Rendering/Renderer.h>

#include "Config.h"

namespace osen {
	class Application
	{
	public:
		Application(EngineConfig config);
		virtual ~Application();
		void run();
	protected:

	private:
		bool m_isRunning = true;

		std::unique_ptr<Window> m_window;
		std::unique_ptr<Renderer> m_renderer;
	};

}