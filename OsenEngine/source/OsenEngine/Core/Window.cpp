
#include "Window.h"
#include <OsenEngine/Platform/GLFWWindow/GLFWWindow.h>

namespace osen
{

	std::unique_ptr<Window> Window::createWindow(WindowConfig config)
	{
		return std::make_unique<GLFWWindow>(config);
	}

}