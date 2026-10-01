#pragma once

#include <string>

namespace osen
{

	struct WindowConfig
	{
		int width = 100;
		int height = 100;
		std::string name = "OsenEngine";

		bool shouldFullscreenOnMaximize = true;
	};

	enum class GraphicsAPI {
		NOT_SPECIFIED,
		VULKAN
	};

	struct RendererConfig
	{
		GraphicsAPI api = GraphicsAPI::NOT_SPECIFIED;
	};

	struct EngineConfig
	{
		WindowConfig windowConfig;
		RendererConfig rendererConfig;
	};


}