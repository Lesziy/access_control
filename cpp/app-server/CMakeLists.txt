cmake_minimum_required(VERSION 3.2)
project(app_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories("../include" "../shared")

file(GLOB SOURCES "src/*.cpp" "../include/*.cpp" "../shared/*.cpp")

set(LOG_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Log)

file(MAKE_DIRECTORY ${LOG_DIR})

if(NOT EXISTS $LOG_FILE)
    file(WRITE "${LOG_DIR}/serverLog.log")
endif()

file(GLOB CONFIG "*.json")
file( COPY ${CONFIG} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )

add_executable(app_server ${SOURCES})