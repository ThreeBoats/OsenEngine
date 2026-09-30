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

		vk::raii::Context m_context;
		vk::raii::Instance m_instance = nullptr;
	};
}