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

	osen::Engine m_engine;
};