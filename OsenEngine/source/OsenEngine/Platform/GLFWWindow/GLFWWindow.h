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

		void processEvents() override;
	protected:

	private:
		void setCallbacks();
		WindowConfig m_config;
		GLFWwindow* m_window;
	};

}