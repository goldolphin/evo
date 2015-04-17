cmake_minimum_required(VERSION 3.1)
project(evo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/test/array.c)

add_executable(evo ${SOURCE_FILES})