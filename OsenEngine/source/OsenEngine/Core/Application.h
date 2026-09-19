#include "Engine.h"
#include <chrono>

namespace osen {
	class Application;
}

class osen::Application
{
public:
	Application();
	virtual ~Application();
	void run();
protected:

private:
	bool m_isRunning = true;

	std::chrono::steady_clock::time_point m_previousFrameTime;

	osen::Engine m_engine;
};