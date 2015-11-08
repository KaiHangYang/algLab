cmake_minimum_required(VERSION 3.3)
project(alglab)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp utility.cpp utility.h)
add_executable(alglab ${SOURCE_FILES})