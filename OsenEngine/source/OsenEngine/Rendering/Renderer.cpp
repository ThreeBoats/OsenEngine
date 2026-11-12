#include "Renderer.h"

#include <OsenEngine/Core/Window.h>
#include <OsenEngine/Core/asserts.h>
#include "logging/RenderingLogger.h"

#include "RenderDevice.h"

namespace osen
{
	Renderer::Renderer(EngineConfig config)
	{
		RenderDevice::Api graphicsApi = RenderDevice::Api::NONE;
		if (config.rendererConfig.api == GraphicsAPI::VULKAN)
			graphicsApi = RenderDevice::Api::VULKAN;

		m_renderDevice = std::make_unique<RenderDevice>(graphicsApi);
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::tmpDraw()
	{

	}
}