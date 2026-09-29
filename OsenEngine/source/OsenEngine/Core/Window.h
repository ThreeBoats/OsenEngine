#pragma once

#include <memory>
#include <string>
#include "Logging/internal/coreLogger.h"


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
		static std::unique_ptr<Window> create(WindowConfig config);

		virtual void processEvents() { coreLogger.log(osen::Logger::LogSeverity::ERROR,
			"this window does not have a process events function"); };
	protected:

	private:

	};

} // namespace osen