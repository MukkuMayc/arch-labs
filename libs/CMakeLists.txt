cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 11)

add_library(matrix src/matrix.c)
add_library(microtime src/microtime.c)