#pragma once

namespace osen
{

	class RenderDevice
	{
	public:
		enum class Api
		{
			NONE,
			VULKAN
		};

		RenderDevice(Api api);
	private:

		Api m_api;
	};

}