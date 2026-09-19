cmake_minimum_required(VERSION 4.4)

project(OsenEngineProject)

set(CMAKE_CONFIGURATION_TYPES
    Debug
    Development
    Release
    CACHE STRING "Available build configurations"
    FORCE
)

add_subdirectory(OsenEngine)
add_subdirectory(Sandbox)
