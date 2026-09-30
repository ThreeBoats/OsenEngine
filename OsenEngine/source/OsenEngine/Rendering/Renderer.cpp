#include "Renderer.h"

#include <OsenEngine/Platform/Vulkan/VulkanRenderer.h>

namespace osen
{

	std::unique_ptr<Renderer> Renderer::create()
	{
		return std::make_unique<VulkanRenderer>();
	}
}