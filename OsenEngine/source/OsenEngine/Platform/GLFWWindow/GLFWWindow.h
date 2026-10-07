#pragma once

#include <OsenEngine/Core/Window.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <OsenEngine/Core/Config.h>
#include <OsenEngine/Platform/Vulkan/VulkanWindow.h>
#include <vector>

namespace osen
{

	class GLFWWindow final : public Window, public VulkanWindow
	{
	public:
		GLFWWindow(EngineConfig config);
		~GLFWWindow();

		void processEvents() override;

		//vulkan only
		std::vector<const char*> requiredVulkanExtensions() const override;
		vk::raii::SurfaceKHR createSurface(const vk::raii::Instance& instance) const override;
	protected:

	private:
		void setCallbacks();
		EngineConfig m_config;
		GLFWwindow* m_window;
	};

}