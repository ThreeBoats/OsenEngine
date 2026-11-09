cmake_minimum_required(VERSION 3.13)

project(OsenEngineProject)

set(CMAKE_CONFIGURATION_TYPES
    Debug
    Development
    Release
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(OsenEngine)
add_subdirectory(Sandbox)
