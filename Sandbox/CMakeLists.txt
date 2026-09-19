
add_executable(Sandbox source/main.cpp "source/GameLog.h" "source/GameLog.cpp")

target_link_libraries(Sandbox PRIVATE OsenEngine)