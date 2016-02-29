cmake_minimum_required(VERSION 3.3)
project(resample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("/usr/local/opt/glm")

set(SOURCE_FILES resample.cpp grid.h)
add_executable(resample ${SOURCE_FILES})