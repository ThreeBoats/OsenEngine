#pragma once

#include <OsenEngine/Core/Window.h>
#include <GLFW/glfw3.h>

#include <OsenEngine/Core/Config.h>
#include <vector>

namespace osen
{

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(EngineConfig config);
		~GLFWWindow();

		void processEvents() override;

		std::vector<const char*> requiredVulkanExtensions() const override;
	protected:

	private:
		void setCallbacks();
		EngineConfig m_config;
		GLFWwindow* m_window;
	};

}