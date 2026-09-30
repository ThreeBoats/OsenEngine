#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "VulkanRenderer.h"


#include "logging/VulkanLogger.h"

namespace osen
{
	VulkanRenderer::VulkanRenderer()
	{
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "vulkan constructor called");

		createInstance();
	}


	void VulkanRenderer::createInstance()
	{
		vk::ApplicationInfo appInfo
		{
			.pApplicationName = "Sandbox",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "OsenEngine",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = vk::ApiVersion14
		};

		vk::InstanceCreateInfo createInfo
		{
			.pApplicationInfo = &appInfo
		};

		m_instance = vk::raii::Instance(m_context, createInfo);
	}
}