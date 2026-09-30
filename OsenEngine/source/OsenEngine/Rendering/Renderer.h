#pragma once

#include <memory>

namespace osen
{
	class Renderer
	{
	public:
		static std::unique_ptr<Renderer> create();


		//the default because otherwise I get linker error. 
		virtual ~Renderer() = default;
	};
}