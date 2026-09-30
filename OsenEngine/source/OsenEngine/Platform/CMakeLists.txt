add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Vulkan")

target_sources(OsenEngine PRIVATE
	
"GLFWWindow/GLFWWINDOW.cpp" "GLFWWindow/GLFWWindow.h"

 "GLFWWindow/GLFWLogger.h" "GLFWWindow/GLFWLogger.cpp" 
 )