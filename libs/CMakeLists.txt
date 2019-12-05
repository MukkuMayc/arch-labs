cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)

add_library(matrix src/matrix.cpp)

target_include_directories(matrix PUBLIC include)