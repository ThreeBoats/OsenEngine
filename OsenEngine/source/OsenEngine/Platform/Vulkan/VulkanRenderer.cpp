#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "VulkanRenderer.h"

#include <string>

#include "logging/VulkanLogger.h"

namespace osen
{
	VulkanRenderer::VulkanRenderer(EngineConfig config, const Window& window)
		:m_window(window)
	{
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "vulkan constructor called");

#ifdef OSEN_DEBUG
		validationLayersEnabled = true;
#endif // OSEN_DEBUG

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

		//TMP TODO: change
		auto physicalDevices = m_instance.enumeratePhysicalDevices();

		for (auto device : physicalDevices)
		{
			auto info = device.getProperties2();

			LOG(VulkanLogger, Logger::LogSeverity::INFO, "Device Info");
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    api version: ") + std::to_string(info.properties.apiVersion));
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    device id: ") + std::to_string(info.properties.deviceID));
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    device name: ") + std::string(info.properties.deviceName.data()));
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    device type: ") + std::to_string(static_cast<int>(info.properties.deviceType)));
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    driver version: ") + std::to_string(info.properties.driverVersion));
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    max image dimension: ") + std::to_string(info.properties.limits.maxImageDimension2D));
			LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    vendor id: ") + std::to_string(info.properties.vendorID));
		}

		auto extensions = m_context.enumerateInstanceExtensionProperties();
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "vulkan extension: ");
		for (auto extension : extensions)
		{
			LOG(VulkanLogger, Logger::LogSeverity::INFO, extension.extensionName);
		}

		m_window.requiredVulkanExtensions();
	}

}//namespace osen