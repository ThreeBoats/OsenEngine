#include "Renderer.h"

#include <OsenEngine/Platform/Vulkan/VulkanRenderer.h>
#include <OsenEngine/Core/Window.h>

namespace osen
{

	std::unique_ptr<Renderer> Renderer::create(EngineConfig config, const Window& window)
	{
		return std::make_unique<VulkanRenderer>(config, window);
	}
}