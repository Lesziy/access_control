cmake_minimum_required(VERSION 3.2)
project(terminal_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("../include" "../shared")

file(GLOB SOURCES "reservations_passing.cpp" "../include/*.cpp" "../shared/*.cpp")

add_executable(snippets ${SOURCES})