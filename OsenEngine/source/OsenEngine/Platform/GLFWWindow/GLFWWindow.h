#pragma once

#include <OsenEngine/Core/Window.h>

#include <GLFW/glfw3.h>

namespace osen
{

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(WindowConfig config);
		~GLFWWindow();
	protected:

	private:
		GLFWwindow* m_window;
	};

}