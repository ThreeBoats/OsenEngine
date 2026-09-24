#include "GLFWWindow.h"

#include <OsenEngine/Core/asserts.h>

osen::GLFWWindow::GLFWWindow(WindowConfig config)
{
	bool succeeded = glfwInit();

	OSEN_ASSERT(succeeded, "glfw did not succeed initializing");


	m_window = glfwCreateWindow(config.width, config.height, config.name.c_str(), NULL, NULL);

	OSEN_ASSERT(m_window != nullptr, "glfw did not create window");
	glfwMakeContextCurrent(m_window);
}

osen::GLFWWindow::~GLFWWindow()
{
	glfwTerminate();
}