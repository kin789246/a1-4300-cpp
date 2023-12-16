cmake_minimum_required(VERSION 3.15)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-result -pedantic-errors")

project(
    a1
    LANGUAGES CXX
    VERSION 0.1.0
)

add_subdirectory(dependencies)
add_subdirectory(src)
