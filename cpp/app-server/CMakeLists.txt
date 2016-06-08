cmake_minimum_required(VERSION 3.2)
project(app_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories("../include" "../shared")

file(GLOB SOURCES "src/*.cpp" "../include/*.cpp" "../shared/*.cpp")

file(GLOB CONFIG "*.json")
file( COPY ${CONFIG} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

add_executable(app_server ${SOURCES})