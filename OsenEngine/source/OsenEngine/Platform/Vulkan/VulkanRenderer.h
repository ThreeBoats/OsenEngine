#pragma once

#include <OsenEngine/Rendering/Renderer.h>
#include <vector>
#include <vulkan/vulkan_raii.hpp>
#include "VulkanWindow.h"

namespace osen
{
	class VulkanRenderer : public Renderer
	{
	public:
		VulkanRenderer(EngineConfig config, const Window& window);
		void tmpDraw() override;

	private:
		void createInstance();
		void createSurface();

		void pickPhysicalDevice();
		void checkPhysicalDevice(const vk::raii::PhysicalDevice& device);
		void printGpuData();
		void createLogicalDevice();

		void createSwapchain();
		void recreateSwapchain();

		void createGraphicsPipeline();
		vk::raii::ShaderModule createShaderModule(const std::vector<char>& code);

		void createCommandPool();
		void createCommandBuffer();

		void createSyncObjects();

		void recordCommandBuffer(unsigned int imageIndex);
		void transition_image_layout(
			unsigned int            imageIndex,
			vk::ImageLayout         old_layout,
			vk::ImageLayout         new_layout,
			vk::AccessFlags2        src_access_mask,
			vk::AccessFlags2        dst_access_mask,
			vk::PipelineStageFlags2 src_stage_mask,
			vk::PipelineStageFlags2 dst_stage_mask);


			bool m_validationLayersEnabled = false;

			/// <summary>
			/// a reference to the window used to render to
			/// </summary>
			const Window& m_window;
			const VulkanWindow* m_vulkanWindow = nullptr;

			vk::raii::Context m_context;
			vk::raii::Instance m_instance = nullptr;
			vk::raii::PhysicalDevice m_physicalDevice = nullptr;
			vk::raii::Device m_device = nullptr;
			vk::raii::Queue m_graphicsQueue = nullptr;
			unsigned int m_queueFamilyIndex = 0;

			vk::raii::SurfaceKHR m_surface = nullptr;
			vk::raii::SwapchainKHR m_swapchain = nullptr;
			vk::Extent2D m_swapchainExtent;
			vk::SurfaceFormatKHR m_swapchainSurfaceFormat;
			std::vector<vk::Image> m_swapchainImages;
			std::vector<vk::raii::ImageView> m_swapchainImageViews;

			vk::raii::PipelineLayout m_pipelineLayout = nullptr;
			vk::raii::Pipeline m_graphicsPipeline = nullptr;

			vk::raii::CommandPool m_commandPool = nullptr;
			vk::raii::CommandBuffer m_commandBuffer = nullptr;

			vk::raii::Semaphore m_presentCompleteSemaphore = nullptr;
			vk::raii::Semaphore m_renderFinishedSemaphore = nullptr;
			vk::raii::Fence m_drawFence = nullptr;
	};
}