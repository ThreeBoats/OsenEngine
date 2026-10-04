#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "VulkanRenderer.h"

#include <string>

#include "logging/VulkanLogger.h"

namespace osen
{
	VulkanRenderer::VulkanRenderer(EngineConfig config, const Window& window)
		:m_window(window)
	{
		LOG(VulkanLogger, Logger::LogSeverity::TRACE, "vulkan constructor called");

#ifdef OSEN_DEBUG
		m_validationLayersEnabled = true;
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

		auto requiredExtensions = m_window.requiredVulkanExtensions();

		//data loss is not a real threat. not enough extensions for that
		uint32_t extensionsAmount = static_cast<uint32_t>(requiredExtensions.size());

		// asks vulkan which extensions are available
		auto availableExtenions = m_context.enumerateInstanceExtensionProperties();

		for (const auto& extension : requiredExtensions)
		{
			bool isSupported = false;

			for (const auto& availableExtension : availableExtenions)
			{
				if (std::strcmp(extension, availableExtension.extensionName) == 0)
				{
					isSupported = true;
					break;
				}
			}

			if (!isSupported)
				LOG(VulkanLogger, Logger::LogSeverity::ERROR, std::string(extension) + std::string(" is not supported"));
			//continue anyways, cause i do not know what to do otherwise
		}

		// the validation layers, for now the standard is good.
		std::vector<const char*> validationLayers;
		if (m_validationLayersEnabled)
			validationLayers.push_back("VK_LAYER_KHRONOS_validation");

		auto availableLayers = m_context.enumerateInstanceLayerProperties();

		for (const auto& validationLayer : validationLayers)
		{
			bool isSupported = false;

			for (const auto& availableLayer : availableLayers)
			{
				if (std::strcmp(validationLayer, availableLayer.layerName) == 0)
				{
					isSupported = true;
					break;
				}
			}

			if (!isSupported)
				LOG(VulkanLogger, Logger::LogSeverity::ERROR, std::string(validationLayer) + std::string(" is not supported"));
			//continue anyways, cause i do not know what to do otherwise
		}

		vk::InstanceCreateInfo createInfo
		{
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()),
			.ppEnabledLayerNames = validationLayers.data(),
			.enabledExtensionCount = extensionsAmount,
			.ppEnabledExtensionNames = requiredExtensions.data()
		};

		LOG(VulkanLogger, Logger::LogSeverity::TRACE, std::string("Vulkan instance created ") +
			(m_validationLayersEnabled ? "with validation layers enabled" : " "));

		m_instance = vk::raii::Instance(m_context, createInfo);
	}

}//namespace osen

//		//TMP TODO: change
//auto physicalDevices = m_instance.enumeratePhysicalDevices();
//
//for (auto device : physicalDevices)
//{
//	auto info = device.getProperties2();
//
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, "Device Info");
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    api version: ") + std::to_string(info.properties.apiVersion));
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    device id: ") + std::to_string(info.properties.deviceID));
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    device name: ") + std::string(info.properties.deviceName.data()));
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    device type: ") + std::to_string(static_cast<int>(info.properties.deviceType)));
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    driver version: ") + std::to_string(info.properties.driverVersion));
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    max image dimension: ") + std::to_string(info.properties.limits.maxImageDimension2D));
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, std::string("    vendor id: ") + std::to_string(info.properties.vendorID));
//}
//
//auto extensions = m_context.enumerateInstanceExtensionProperties();
//LOG(VulkanLogger, Logger::LogSeverity::INFO, "vulkan extension: ");
//for (auto extension : extensions)
//{
//	LOG(VulkanLogger, Logger::LogSeverity::INFO, extension.extensionName);
//}
//
//m_window.requiredVulkanExtensions();