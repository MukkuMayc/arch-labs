cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 11)

include_directories(${PROJECT_SOURCE_DIR}/libs/include)

add_executable(task_1 task_1.c)

target_link_libraries(task_1 matrix microtime)