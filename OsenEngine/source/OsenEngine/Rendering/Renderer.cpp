#include "Renderer.h"

#include <OsenEngine/Platform/Vulkan/VulkanRenderer.h>
#include <OsenEngine/Core/Window.h>
#include <OsenEngine/Core/asserts.h>
#include "logging/RenderingLogger.h"

namespace osen
{

	std::unique_ptr<Renderer> Renderer::create(EngineConfig config, const Window& window)
	{
		switch (config.rendererConfig.api)
		{
		case GraphicsAPI::VULKAN:
			return std::make_unique<VulkanRenderer>(config, window);

		case GraphicsAPI::NOT_SPECIFIED:
			LOG(RenderingLogger, osen::Logger::LogSeverity::ERROR,
				"graphics api not specified, fallback to vulkan");
			return std::make_unique<VulkanRenderer>(config, window);

		default:
			OSEN_ASSERT(false, "Unsupported graphics API");
			return nullptr;
		}
	}
}