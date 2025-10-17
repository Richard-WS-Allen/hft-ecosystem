cmake_minimum_required(VERSION 3.12)

project(HftEcosystem LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory (src)
add_subdirectory (test)
