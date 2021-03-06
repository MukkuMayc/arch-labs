cmake_minimum_required(VERSION 3.15)
project(arch_labs
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

message(STATUS "Checking OpenMP")
find_package(OpenMP)
IF(OPENMP_FOUND)
    message("Found OpenMP!")
    # add flags for OpenMP
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message("Missed OpenMP!")
ENDIF()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstandalone-debug")

set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

link_directories(benchmark benchmark/build/src/)
add_subdirectory(benchmark)

add_subdirectory(libs)
add_subdirectory(task_1)
add_subdirectory(task_2)
add_subdirectory(task_3)
add_subdirectory(task_4)
add_subdirectory(task_5)