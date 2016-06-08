cmake_minimum_required(VERSION 3.2)
project(terminal_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories("../include" "../shared")

file(GLOB SOURCES "*.cpp" "../include/*.cpp" "../shared/*.cpp")

add_executable(snippets ${SOURCES})