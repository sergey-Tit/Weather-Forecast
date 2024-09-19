cmake_minimum_required(VERSION 3.26)
project(labwork7_sergey_Tit)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    set(CMAKE_LIBRARY OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(lib)
add_subdirectory(bin)
