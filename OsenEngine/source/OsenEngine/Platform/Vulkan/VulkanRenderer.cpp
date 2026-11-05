#define VULKAN_HPP_NO_STRUCT_CONSTRUCTORS
#include "VulkanRenderer.h"

#include <string>
#include <filesystem>
#include <fstream>
#include "VulkanWindow.h"

#include "logging/VulkanLogger.h"
#include <OsenEngine/Core/asserts.h>

std::vector<const char*> requiredDeviceExtension = { vk::KHRSwapchainExtensionName };


namespace osen
{
	static std::vector<char> readFile(const std::string& filename);

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
		createSwapchain();
		createGraphicsPipeline();
		createCommandPool();
		createCommandBuffer();
		createSyncObjects();
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
		auto availableExtensions = m_context.enumerateInstanceExtensionProperties();

		for (const auto& extension : requiredExtensions)
		{
			bool isSupported = false;

			for (const auto& availableExtension : availableExtensions)
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

		// Check if the device supports Vulkan 1.4
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
			&& features.template get<vk::PhysicalDeviceVulkan13Features>().synchronization2
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
				const auto memoryMB = heap.size / (1024 * 1024);
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
		m_queueFamilyIndex = 0;

		for (int i = 0; i < queueFamilies.size(); i++)
		{
			if ((queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlags{}
				&& m_physicalDevice.getSurfaceSupportKHR(i, m_surface))
			{
				m_queueFamilyIndex = i;
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
				{.synchronization2 = true, .dynamicRendering = true},            // vk::PhysicalDeviceVulkan13Features
				{.extendedDynamicState = true}       // vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT
		};

		float queuePriority = 1.0f;
		vk::DeviceQueueCreateInfo queueInfo{
			.queueFamilyIndex = m_queueFamilyIndex,
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

		m_graphicsQueue = vk::raii::Queue(m_device, m_queueFamilyIndex, 0);
	}


	void VulkanRenderer::createSwapchain()
	{
		auto surfaceCapabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);

		//we want 3 images for some headroom.
		uint32_t imageCount = std::max(3u, surfaceCapabilities.minImageCount);
		if (surfaceCapabilities.maxImageCount > 0)
		{
			imageCount = std::min(imageCount, surfaceCapabilities.maxImageCount);
		}

		if (surfaceCapabilities.currentExtent.width !=
			std::numeric_limits<uint32_t>::max())
		{
			m_swapchainExtent = surfaceCapabilities.currentExtent;
		}
		else
		{
			const Window::FramebufferSize size = m_window.getFramebufferSize();

			m_swapchainExtent = {
				std::clamp(
					static_cast<uint32_t>(size.width),
					surfaceCapabilities.minImageExtent.width,
					surfaceCapabilities.maxImageExtent.width),
				std::clamp(
					static_cast<uint32_t>(size.height),
					surfaceCapabilities.minImageExtent.height,
					surfaceCapabilities.maxImageExtent.height)
			};
		}

		auto surfaceFormats = m_physicalDevice.getSurfaceFormatsKHR(m_surface);
		OSEN_ASSERT(!surfaceFormats.empty(), "Vulkan: no surface formats available!");
		m_swapchainSurfaceFormat = surfaceFormats[0];

		//for now half copied from the official vulkan tutorial. But simplified
		vk::SwapchainCreateInfoKHR swapchainInfo{
			.surface = m_surface,
			.minImageCount = imageCount,
			.imageFormat = m_swapchainSurfaceFormat.format,
			.imageColorSpace = m_swapchainSurfaceFormat.colorSpace,
			.imageExtent = m_swapchainExtent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlags::BitsType::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.preTransform = surfaceCapabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,  // not checking if this is supported (might result in crash)
			.presentMode = vk::PresentModeKHR::eFifo,				//use this one because it is always available.
			.clipped = true
		};

		m_swapchain = vk::raii::SwapchainKHR{ m_device, swapchainInfo };
		m_swapchainImages = m_swapchain.getImages();

		vk::ImageViewCreateInfo imageViewInfo{
			.viewType = vk::ImageViewType::e2D, //no idea what this does.
			.format = m_swapchainSurfaceFormat.format,
			.subresourceRange = vk::ImageSubresourceRange{
				.aspectMask = vk::ImageAspectFlagBits::eColor, .levelCount = 1, .layerCount = 1}
		};

		for (const auto& image : m_swapchainImages)
		{
			imageViewInfo.image = image;
			vk::raii::ImageView imageView{ m_device, imageViewInfo };
			m_swapchainImageViews.push_back(std::move(imageView));
		}
	}

	void VulkanRenderer::recreateSwapchain()
	{
		m_device.waitIdle();
		m_swapchain = nullptr;
		m_swapchainImages.clear();
		m_swapchainImageViews.clear();

		createSwapchain();
	}

	void VulkanRenderer::createGraphicsPipeline()
	{
		// TODO:
		// WARNING: currently, you have to manually compile and copy the spir-v binary
		// to the working directory. I do not know another way for this, that 
		// does not require a mega-system with assets and all. so for now. this is it.
		// if you compile the program and run it, it will tell you the working directory.
		// copy the produced binary there. this is also in the readme.
		vk::raii::ShaderModule shaderModule = createShaderModule(readFile("slang.spv"));
		vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
			.stage = vk::ShaderStageFlagBits::eVertex,
			.module = shaderModule,
			.pName = "vertMain"
		};

		vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
			.stage = vk::ShaderStageFlagBits::eFragment,
			.module = shaderModule,
			.pName = "fragMain"
		};

		vk::PipelineShaderStageCreateInfo shaderStages[]
			= { vertShaderStageInfo, fragShaderStageInfo };

		std::vector<vk::DynamicState> dynamicStates = {
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor };

		vk::PipelineDynamicStateCreateInfo dynamicState{
			.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
			.pDynamicStates = dynamicStates.data() };

		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{
			.topology = vk::PrimitiveTopology::eTriangleList
		};

		vk::Viewport viewport{
			.width = static_cast<float>(m_swapchainExtent.width),
			.height = static_cast<float>(m_swapchainExtent.height),
			.minDepth = 0,
			.maxDepth = 1
		};

		vk::Rect2D scissor{
			.offset = {0, 0},
			.extent = m_swapchainExtent
		};

		vk::PipelineViewportStateCreateInfo viewportState{
			.viewportCount = 1,
			.pViewports = &viewport,
			.scissorCount = 1,
			.pScissors = &scissor
		};

		//copied from tutorial
		vk::PipelineRasterizationStateCreateInfo rasterizer
		{
			.depthClampEnable = vk::False,
			.rasterizerDiscardEnable = vk::False,
			.polygonMode = vk::PolygonMode::eFill,
			.cullMode = vk::CullModeFlagBits::eNone, // no culling (for now)
			.frontFace = vk::FrontFace::eClockwise,
			.depthBiasEnable = vk::False,
			.lineWidth = 1.0f
		};

		vk::PipelineMultisampleStateCreateInfo multisampling{
			.rasterizationSamples = vk::SampleCountFlagBits::e1,
			.sampleShadingEnable = vk::False };

		vk::PipelineColorBlendAttachmentState colorBlendAttachment{
			.blendEnable = vk::False,
			.colorWriteMask = vk::ColorComponentFlagBits::eR
			| vk::ColorComponentFlagBits::eG
			| vk::ColorComponentFlagBits::eB
			| vk::ColorComponentFlagBits::eA
		};

		vk::PipelineColorBlendStateCreateInfo colorBlending{
			.logicOpEnable = vk::False,
			.logicOp = vk::LogicOp::eCopy,
			.attachmentCount = 1,
			.pAttachments = &colorBlendAttachment };

		vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
			.setLayoutCount = 0, .pushConstantRangeCount = 0 };

		m_pipelineLayout = vk::raii::PipelineLayout(m_device, pipelineLayoutInfo);

		vk::StructureChain<vk::GraphicsPipelineCreateInfo,
			vk::PipelineRenderingCreateInfo> pipelineCreateInfoChain = {
	{.stageCount = 2,
	 .pStages = shaderStages,
	 .pVertexInputState = &vertexInputInfo,
	 .pInputAssemblyState = &inputAssemblyInfo,
	 .pViewportState = &viewportState,
	 .pRasterizationState = &rasterizer,
	 .pMultisampleState = &multisampling,
	 .pColorBlendState = &colorBlending,
	 .pDynamicState = &dynamicState,
	 .layout = m_pipelineLayout,
	 .renderPass = nullptr},
	{.colorAttachmentCount = 1, .pColorAttachmentFormats = &m_swapchainSurfaceFormat.format} };

		m_graphicsPipeline = vk::raii::Pipeline(
			m_device, nullptr, pipelineCreateInfoChain.get<vk::GraphicsPipelineCreateInfo>());
	}

	vk::raii::ShaderModule VulkanRenderer::createShaderModule(const std::vector<char>& code)
	{
		vk::ShaderModuleCreateInfo shaderModuleInfo{
			.codeSize = code.size(),
			.pCode = reinterpret_cast<const uint32_t*>(code.data())
		};

		return vk::raii::ShaderModule(m_device, shaderModuleInfo);
	}

	void VulkanRenderer::createCommandPool()
	{
		vk::CommandPoolCreateInfo createInfo{
			.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = m_queueFamilyIndex
		};

		m_commandPool = vk::raii::CommandPool{ m_device, createInfo };
	}

	void VulkanRenderer::createCommandBuffer()
	{
		vk::CommandBufferAllocateInfo allocateInfo{
			.commandPool = m_commandPool,
			.commandBufferCount = 1
		};
		m_commandBuffer = std::move(vk::raii::CommandBuffers{ m_device, allocateInfo }.front());
	}


	void VulkanRenderer::recordCommandBuffer(unsigned int imageIndex)
	{
		//none of the fields apply, but it is required
		vk::CommandBufferBeginInfo beginInfo{};
		m_commandBuffer.begin(beginInfo);

		transition_image_layout(
			imageIndex,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eColorAttachmentOptimal,
			{},                                                        // srcAccessMask (no need to wait for previous operations)
			vk::AccessFlagBits2::eColorAttachmentWrite,                // dstAccessMask
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,        // srcStage
			vk::PipelineStageFlagBits2::eColorAttachmentOutput         // dstStage
		);

		vk::ClearValue              clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
		vk::RenderingAttachmentInfo attachmentInfo = {
			.imageView = m_swapchainImageViews[imageIndex],
			.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
			.loadOp = vk::AttachmentLoadOp::eClear,  //clear the image before rendering to it
			.storeOp = vk::AttachmentStoreOp::eStore,
			.clearValue = clearColor };

		vk::RenderingInfo renderingInfo = {
			.renderArea = {.offset = {0, 0}, .extent = m_swapchainExtent},
			.layerCount = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments = &attachmentInfo };

		m_commandBuffer.beginRendering(renderingInfo);

		m_commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_graphicsPipeline);
		m_commandBuffer.setViewport(0,
			vk::Viewport(0.0f, 0.0f, static_cast<float>(m_swapchainExtent.width),
				static_cast<float>(m_swapchainExtent.height), 0.0f, 1.0f));
		m_commandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), m_swapchainExtent));

		///here it is !!!
		m_commandBuffer.draw(3, 1, 0, 0);

		m_commandBuffer.endRendering();

		transition_image_layout(
			imageIndex,
			vk::ImageLayout::eColorAttachmentOptimal,
			vk::ImageLayout::ePresentSrcKHR,
			vk::AccessFlagBits2::eColorAttachmentWrite,             // srcAccessMask
			{},                                                     // dstAccessMask
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,     // srcStage
			vk::PipelineStageFlagBits2::eBottomOfPipe               // dstStage
		);

		m_commandBuffer.end();
	}

	void VulkanRenderer::createSyncObjects()
	{
		m_presentCompleteSemaphore = vk::raii::Semaphore(m_device, vk::SemaphoreCreateInfo());
		m_renderFinishedSemaphore = vk::raii::Semaphore(m_device, vk::SemaphoreCreateInfo());
		m_drawFence = vk::raii::Fence(m_device, { .flags = vk::FenceCreateFlagBits::eSignaled });

	}

	void VulkanRenderer::tmpDraw()
	{
		auto fenceResult = m_device.waitForFences(*m_drawFence, vk::True, UINT64_MAX);
		if (fenceResult != vk::Result::eSuccess)
			LOG(VulkanLogger, Logger::LogSeverity::ERROR, "failed to wait for draw fence");


		auto [result, imageIndex] = m_swapchain.acquireNextImage(
			UINT64_MAX, *m_presentCompleteSemaphore, nullptr);

		bool dirtySwapchain = false;

		if (result == vk::Result::eErrorOutOfDateKHR)
		{
			recreateSwapchain();
			return;
		}
		else if (result == vk::Result::eSuboptimalKHR)
			dirtySwapchain = true;
		else if (result != vk::Result::eSuccess)
			LOG(VulkanLogger, Logger::LogSeverity::ERROR, "failed to acquire next image");

		m_device.resetFences(*m_drawFence);

		recordCommandBuffer(imageIndex);
		m_graphicsQueue.waitIdle();

		vk::PipelineStageFlags waitDestinationStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
		vk::SubmitInfo submitInfo{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &*m_presentCompleteSemaphore,
			.pWaitDstStageMask = &waitDestinationStageMask,
			.commandBufferCount = 1,
			.pCommandBuffers = &*m_commandBuffer,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &*m_renderFinishedSemaphore
		};


		m_graphicsQueue.submit(submitInfo, *m_drawFence);

		m_graphicsQueue.waitIdle();

		const vk::PresentInfoKHR presentInfoKHR{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &*m_renderFinishedSemaphore,
			.swapchainCount = 1,
			.pSwapchains = &*m_swapchain,
			.pImageIndices = &imageIndex };

		result = m_graphicsQueue.presentKHR(presentInfoKHR);

		if (dirtySwapchain)
			recreateSwapchain();

	}

	void VulkanRenderer::transition_image_layout(
		unsigned int            imageIndex,
		vk::ImageLayout         old_layout,
		vk::ImageLayout         new_layout,
		vk::AccessFlags2        src_access_mask,
		vk::AccessFlags2        dst_access_mask,
		vk::PipelineStageFlags2 src_stage_mask,
		vk::PipelineStageFlags2 dst_stage_mask)
	{
		vk::ImageMemoryBarrier2 barrier = {
			.srcStageMask = src_stage_mask,
			.srcAccessMask = src_access_mask,
			.dstStageMask = dst_stage_mask,
			.dstAccessMask = dst_access_mask,
			.oldLayout = old_layout,
			.newLayout = new_layout,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = m_swapchainImages[imageIndex],
			.subresourceRange = {
				   .aspectMask = vk::ImageAspectFlagBits::eColor,
				   .baseMipLevel = 0,
				   .levelCount = 1,
				   .baseArrayLayer = 0,
				   .layerCount = 1} };
		vk::DependencyInfo dependency_info = {
			.dependencyFlags = {},
			.imageMemoryBarrierCount = 1,
			.pImageMemoryBarriers = &barrier };
		m_commandBuffer.pipelineBarrier2(dependency_info);
	}

	static std::vector<char> readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			LOG(VulkanLogger, Logger::LogSeverity::ERROR, "cannot open file: " + filename);
		}
		std::vector<char> buffer(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
		file.close();
		return buffer;
	}

}//namespace osen