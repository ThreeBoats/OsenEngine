#pragma once

#include <OsenEngine/Rendering/Renderer.h>

#include <vulkan/vulkan_raii.hpp>

namespace osen
{
	class VulkanRenderer : public Renderer
	{
	public:
		VulkanRenderer();

	private:

		void createInstance();

		bool validationLayersEnabled = false;

		vk::raii::Context m_context;
		vk::raii::Instance m_instance = nullptr;
	};
}