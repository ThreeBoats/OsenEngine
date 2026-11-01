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

		struct FramebufferSize {
			int width = -1;
			int height = -1;

			FramebufferSize(int width, int height) : width(width), height(height) {}
		};

		virtual FramebufferSize getFramebufferSize() const = 0;

	};

} // namespace osen