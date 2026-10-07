#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "VulkanRenderer.h"

#include <string>
#include "VulkanWindow.h"

#include "logging/VulkanLogger.h"

std::vector<const char*> requiredDeviceExtension = { vk::KHRSwapchainExtensionName };

namespace osen
{
	VulkanRenderer::VulkanRenderer(EngineConfig config, const Window& window)
		:m_window(window)
	{
		LOG(VulkanLogger, Logger::LogSeverity::TRACE, "constructor called");

		m_vulkanWindow = dynamic_cast<const VulkanWindow*>(&window);

#ifdef OSEN_DEBUG
		m_validationLayersEnabled = true;
#endif // OSEN_DEBUG

		createInstance();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
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

		auto requiredExtensions = m_vulkanWindow->requiredVulkanExtensions();

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

		LOG(VulkanLogger, Logger::LogSeverity::TRACE, std::string("instance created ") +
			(m_validationLayersEnabled ? "with validation layers enabled" : " "));

		m_instance = vk::raii::Instance(m_context, createInfo);
	}

	void VulkanRenderer::createSurface()
	{
		m_surface = m_vulkanWindow->createSurface(m_instance);
	}

	void VulkanRenderer::pickPhysicalDevice()
	{
		auto physicalDevices = m_instance.enumeratePhysicalDevices();

		if (physicalDevices.empty())
			LOG(VulkanLogger, Logger::LogSeverity::FATAL, "no devices with Vulkan support");

		//loop through all devices, if one is suitable, set THE physical device to it. and continue
		// with new checks. If again a new one is found, it overwrites the previous.
		for (const auto& device : physicalDevices)
		{
			checkPhysicalDevice(device);
		}

		if (m_physicalDevice == nullptr)
			LOG(VulkanLogger, Logger::LogSeverity::FATAL, "failed to find a suitable GPU");

		printGpuData();
	}

	// I really hope I do not have to touch this function again.
	void VulkanRenderer::checkPhysicalDevice(const vk::raii::PhysicalDevice& device)
	{

		bool hasGraphicsQueue = false;
		bool supportsVulkan14 = false;
		bool supportsAllRequiredExtensions = false;
		bool supportsRequiredFeatures = false;

		// Check if the device supports Vulkan 1.3
		auto properties = device.getProperties2();

		if (properties.properties.apiVersion >= vk::ApiVersion14)
			supportsVulkan14 = true;

		// Check if the device has a graphics queue
		auto queueFamilies = device.getQueueFamilyProperties2();

		for (const auto& queue : queueFamilies)
		{
			if (queue.queueFamilyProperties.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				hasGraphicsQueue = true;
				break;
			}
		}

		// Check if all required extensions are supported.
		auto availableDeviceExtensions = device.enumerateDeviceExtensionProperties();

		supportsAllRequiredExtensions = true;

		for (const auto& requiredExtension : requiredDeviceExtension)
		{
			bool extensionFound = false;

			for (const auto& availableExtension : availableDeviceExtensions)
			{
				if (std::strcmp(availableExtension.extensionName, requiredExtension) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			if (!extensionFound)
			{
				supportsAllRequiredExtensions = false;
				break;
			}
		}

		// Check if the required features are supported
		auto features = device.getFeatures2<
			vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceVulkan11Features,
			vk::PhysicalDeviceVulkan13Features,
			vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

		if (features.template get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters
			&& features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering
			&& features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState)
		{
			supportsRequiredFeatures = true;
		}

		// Select this device only if it satisfies all requirements
		if (hasGraphicsQueue && supportsVulkan14 &&
			supportsAllRequiredExtensions && supportsRequiredFeatures)
		{
			m_physicalDevice = device;
			return;
		}
	}
	void VulkanRenderer::printGpuData()
	{
		auto properties = m_physicalDevice.getProperties();
		auto memoryProperties = m_physicalDevice.getMemoryProperties();

		std::string gpuName(properties.deviceName.data());
		std::string gpuType = "Unknown";

		switch (properties.deviceType)
		{
		case vk::PhysicalDeviceType::eDiscreteGpu:
			gpuType = "Discrete GPU";
			break;

		case vk::PhysicalDeviceType::eIntegratedGpu:
			gpuType = "Integrated GPU";
			break;

		case vk::PhysicalDeviceType::eVirtualGpu:
			gpuType = "Virtual GPU";
			break;

		case vk::PhysicalDeviceType::eCpu:
			gpuType = "CPU";
			break;
		}

		std::string apiVersion =
			std::to_string(VK_VERSION_MAJOR(properties.apiVersion)) + "." +
			std::to_string(VK_VERSION_MINOR(properties.apiVersion)) + "." +
			std::to_string(VK_VERSION_PATCH(properties.apiVersion));

		LOG(VulkanLogger, Logger::LogSeverity::INFO, "GPU: " + gpuName);
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "    GPU type: " + gpuType);
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "    Vendor ID: " + std::to_string(properties.vendorID));
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "    Device ID: " + std::to_string(properties.deviceID));
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "    Driver version: " + std::to_string(properties.driverVersion));
		LOG(VulkanLogger, Logger::LogSeverity::INFO, "    Vulkan API version: " + apiVersion);

		for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
		{
			const auto& heap = memoryProperties.memoryHeaps[i];

			if (heap.flags & vk::MemoryHeapFlagBits::eDeviceLocal)
			{
				const auto memoryMB = heap.size / (1024ull * 1024ull);
				const std::string memoryMessage =
					"GPU memory heap " + std::to_string(i) + ": " +
					std::to_string(memoryMB) + " MB";

				LOG(VulkanLogger, Logger::LogSeverity::INFO, "    " + memoryMessage);
			}
		}
	}


	/*
	here we creat the logical device. this is used to interface with the gpu later
	we choose a queuefamily here. one gpu can have multiple. we check every queue family
	and use one that has at least a graphics queue.
	after that we tell the device to use some features that are not standard in vulkan
	because they came in later updater or extensions
	*/
	void VulkanRenderer::createLogicalDevice()
	{
		std::vector<vk::QueueFamilyProperties> queueFamilies = m_physicalDevice.getQueueFamilyProperties();
		unsigned int queueFamilyIndex = 0;

		for (int i = 0; i < queueFamilies.size(); i++)
		{
			if ((queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlags{})
			{
				queueFamilyIndex = i;
				break;
			}
		}

		vk::StructureChain<vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceVulkan11Features,
			vk::PhysicalDeviceVulkan13Features,
			vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
			featureChain = {
				{},                                    // vk::PhysicalDeviceFeatures2
				{.shaderDrawParameters = true},        // vk::PhysicalDeviceVulkan11Features
				{.dynamicRendering = true},            // vk::PhysicalDeviceVulkan13Features
				{.extendedDynamicState = true}         // vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
		};

		float queuePriority = 1.0f;
		vk::DeviceQueueCreateInfo queueInfo{
			.queueFamilyIndex = queueFamilyIndex,
			.queueCount = 1,
			.pQueuePriorities = &queuePriority
		};

		vk::DeviceCreateInfo deviceInfo
		{
			.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queueInfo,
			.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtension.size()),
			.ppEnabledExtensionNames = requiredDeviceExtension.data()
		};
		m_device = vk::raii::Device{ m_physicalDevice, deviceInfo };

		m_graphicsQueue = vk::raii::Queue(m_device, queueFamilyIndex, 0);
	}
}//namespace osen