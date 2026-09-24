#pragma once

#include <memory>
#include <string>

namespace osen
{

	//
	struct WindowConfig
	{
		int width = 100;
		int height = 100;
		std::string name = "OsenEngine";
	};

	class Window
	{
	public:
		static std::unique_ptr<Window> createWindow(WindowConfig config);
	protected:

	private:

	};

} // namespace osen