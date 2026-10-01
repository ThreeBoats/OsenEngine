
#include "Window.h"
#include <OsenEngine/Platform/GLFWWindow/GLFWWindow.h>

namespace osen
{

	std::unique_ptr<Window> Window::create(EngineConfig config)
	{
		return std::make_unique<GLFWWindow>(config);
	}

}