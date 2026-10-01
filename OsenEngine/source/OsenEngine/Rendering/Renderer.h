#pragma once

#include <memory>
#include <OsenEngine/Core/Config.h>

#include <OsenEngine/Core/Window.h>

namespace osen
{
	

	class Renderer
	{
	public:
		static std::unique_ptr<Renderer> create(EngineConfig config, const Window& window);


		//the default because otherwise I get linker error. 
		virtual ~Renderer() = default;
	};
}