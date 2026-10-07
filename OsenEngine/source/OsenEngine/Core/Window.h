#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Logging/internal/coreLogger.h"

#include "Config.h"

namespace osen
{
	class Window
	{
	public:
		static std::unique_ptr<Window> create(EngineConfig config);

		virtual ~Window() = default;

		virtual void processEvents() {
			coreLogger.log(osen::Logger::LogSeverity::ERROR,
				"this window does not have a process events function");
		};

	};

} // namespace osen