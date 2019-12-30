cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

add_library(task_3_lib task_3_lib.cpp)
target_link_libraries(task_3_lib matrix)

add_executable(task_3 task_3.cpp)
target_link_libraries(task_3 matrix task_3_lib)

add_executable(task_3_bm task_3_bm.cpp)
target_link_libraries(task_3_bm matrix task_3_lib benchmark)