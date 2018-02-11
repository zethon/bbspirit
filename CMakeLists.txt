cmake_minimum_required(VERSION 3.0)

project(bbspirit)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(bbspirit bbspirit.cpp)