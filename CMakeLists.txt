cmake_minimum_required(VERSION 3.28)
project(labwork10)

set(CMAKE_CXX_STANDARD 20)

add_executable(labwork10 main.cpp)

add_subdirectory(lib)
add_subdirectory(tests)

enable_testing()
