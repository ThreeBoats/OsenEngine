#pragma once

#include <OsenEngine/Rendering/Renderer.h>

#include <vulkan/vulkan_raii.hpp>

namespace osen
{
	class VulkanRenderer : public Renderer
	{
	public:
		VulkanRenderer(EngineConfig config, const Window& window);

	private:
		void createInstance();
		void pickPhysicalDevice();
		void checkPhysicalDevice(const vk::raii::PhysicalDevice& device);
		void printGpuData();

		bool m_validationLayersEnabled = false;

		/// <summary>
		/// a reference to the window used to render to
		/// </summary>
		const Window& m_window;

		/// <summary>
		/// the vulkan context, used for: 
		/// </summary>
		vk::raii::Context m_context;

		/// <summary>
		/// the vulkan instance, used for: 
		/// </summary>
		vk::raii::Instance m_instance = nullptr;
		vk::raii::PhysicalDevice m_physicalDevice = nullptr;

		/// <summary>
		/// the physical device we are using.
		/// </summary>
		//vk::raii::PhysicalDevice m_physicalDevice;
	};
}