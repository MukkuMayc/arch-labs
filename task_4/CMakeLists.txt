cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)


add_library(task_4_lib task_4_lib.cpp)
target_link_libraries(task_4_lib matrix)

add_executable(task_4 task_4.cpp)
target_link_libraries(task_4 matrix task_4_lib)

add_executable(task_4_bm task_4_bm.cpp)
target_link_libraries(task_4_bm matrix task_4_lib benchmark)