cmake_minimum_required(VERSION 3.2)
project(app_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories("../include" "../shared")

file(GLOB SOURCES "src/*.cpp" "../include/*.cpp" "../shared/*.cpp")

add_executable(app_server ${SOURCES})