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

		bool m_validationLayersEnabled = false;

		/// <summary>
		/// a reference to the window used to render to
		/// </summary>
		const Window& m_window;

		vk::raii::Context m_context;
		vk::raii::Instance m_instance = nullptr;
	};
}