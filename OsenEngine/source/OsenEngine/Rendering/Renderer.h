#pragma once

#include <memory>
#include <OsenEngine/Core/Config.h>

#include <OsenEngine/Core/Window.h>


namespace osen
{
class RenderDevice;

	class Renderer
	{
	public:
		Renderer(EngineConfig config);
		~Renderer();

		void tmpDraw();
	private:
		std::unique_ptr<RenderDevice> m_renderDevice;

	};
}