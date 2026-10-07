#include "GLFWWindow.h"

#include <OsenEngine/Core/asserts.h>
#include "GLFWLogger.h"

#include <OsenEngine/Core/Config.h>

#include <GLFW/glfw3.h>

osen::GLFWWindow::GLFWWindow(EngineConfig config)
{
	m_config = config;

	bool succeeded = glfwInit();

	OSEN_ASSERT(succeeded, "glfw did not succeed initializing");

	LOG(GLFWLogger, Logger::LogSeverity::INFO, "window created");

	// if we use vulkan we do not want a openGL context.
	if (config.rendererConfig.api == GraphicsAPI::VULKAN)
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_window = glfwCreateWindow(config.windowConfig.width, config.windowConfig.height, config.windowConfig.name.c_str(), nullptr, nullptr);

	OSEN_ASSERT(m_window != nullptr, "glfw did not create window");

	setCallbacks();
}

osen::GLFWWindow::~GLFWWindow()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void osen::GLFWWindow::processEvents()
{
	glfwPollEvents();
}

std::vector<const char*> osen::GLFWWindow::requiredVulkanExtensions() const
{
	OSEN_ASSERT(m_config.rendererConfig.api == GraphicsAPI::VULKAN,
		"vulkan specific function called while not using vulkan.");

	uint32_t count = 0;
	const char** extensions =
		glfwGetRequiredInstanceExtensions(&count);

	if (extensions == nullptr) {
		return {};
	}

	return {
		extensions,
		extensions + count
	};
}

vk::raii::SurfaceKHR osen::GLFWWindow::createSurface(const vk::raii::Instance& instance) const
{
	VkSurfaceKHR rawSurface = VK_NULL_HANDLE;

	const VkResult result = glfwCreateWindowSurface(static_cast<VkInstance>(*instance),
		m_window,
		nullptr,
		&rawSurface);

	return vk::raii::SurfaceKHR(instance, rawSurface);
}

void osen::GLFWWindow::setCallbacks()
{
	glfwSetErrorCallback(
		[](int error, const char* description)
		{
			LOG(GLFWLogger, Logger::LogSeverity::ERROR, "GLFW error: " + std::to_string(error) + " " + description);
		}
	);

	glfwSetWindowSizeCallback(
		m_window,
		[](GLFWwindow* window, int width, int height)
		{
			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Window resized: " +
				std::to_string(width) + "x" +
				std::to_string(height)
			);
		}
	);

	glfwSetWindowMaximizeCallback(m_window,
		[](GLFWwindow* window, int maximized)
		{
			glfwSetWindowMonitor(window,
				glfwGetPrimaryMonitor(),
				0, 0,
				glfwGetVideoMode(glfwGetPrimaryMonitor())->width,
				glfwGetVideoMode(glfwGetPrimaryMonitor())->height,
				glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
		});

	glfwSetWindowCloseCallback(
		m_window,
		[](GLFWwindow* window)
		{
			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Window close requested"
			);

			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	);

	glfwSetKeyCallback(
		m_window,
		[](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			const char* actionName = "UNKNOWN";

			switch (action)
			{
			case GLFW_PRESS:
				actionName = "PRESS";
				break;

			case GLFW_RELEASE:
				actionName = "RELEASE";
				break;

			case GLFW_REPEAT:
				actionName = "REPEAT";
				break;
			}

			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Key event - key: " +
				std::to_string(key) +
				", scancode: " +
				std::to_string(scancode) +
				", action: " +
				actionName +
				", mods: " +
				std::to_string(mods)
			);
		}
	);

	glfwSetCharCallback(
		m_window,
		[](GLFWwindow* window, unsigned int codepoint)
		{
			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Character input: " +
				std::to_string(codepoint)
			);
		}
	);

	glfwSetMouseButtonCallback(
		m_window,
		[](GLFWwindow* window, int button, int action, int mods)
		{
			const char* actionName = "UNKNOWN";

			switch (action)
			{
			case GLFW_PRESS:
				actionName = "PRESS";
				break;

			case GLFW_RELEASE:
				actionName = "RELEASE";
				break;
			}

			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Mouse button event - button: " +
				std::to_string(button) +
				", action: " +
				actionName +
				", mods: " +
				std::to_string(mods)
			);
		}
	);

	glfwSetScrollCallback(
		m_window,
		[](GLFWwindow* window, double xOffset, double yOffset)
		{
			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Scroll event - x: " +
				std::to_string(xOffset) +
				", y: " +
				std::to_string(yOffset)
			);
		}
	);

	glfwSetCursorPosCallback(
		m_window,
		[](GLFWwindow* window, double xPos, double yPos)
		{
			LOG(
				GLFWLogger,
				Logger::LogSeverity::TRACE,
				"Cursor position - x: " +
				std::to_string(xPos) +
				", y: " +
				std::to_string(yPos)
			);
		}
	);
}
