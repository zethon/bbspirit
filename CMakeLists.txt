cmake_minimum_required(VERSION 3.0)

project(bbspirit)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MACOSX_RPATH 1)

# initialize conan libs
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS KEEP_RPATHS)

# optional configuration
option(BBSPIRIT_TEST "Build unit tests (default OFF)" OFF)

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

if (BBSPIRIT_TEST)
    enable_testing()
    add_subdirectory(tests)
endif (BBSPIRIT_TEST)