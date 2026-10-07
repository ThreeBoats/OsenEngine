#pragma once

#include <OsenEngine/Rendering/Renderer.h>

#include <vulkan/vulkan_raii.hpp>
#include "VulkanWindow.h"

namespace osen
{
	class VulkanRenderer : public Renderer
	{
	public:
		VulkanRenderer(EngineConfig config, const Window& window);

	private:
		void createInstance();
		void createSurface();
		void pickPhysicalDevice();
		void checkPhysicalDevice(const vk::raii::PhysicalDevice& device);
		void printGpuData();
		void createLogicalDevice();

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

		vk::raii::SurfaceKHR m_surface = nullptr;
	};
}