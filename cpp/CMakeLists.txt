cmake_minimum_required(VERSION 3.2)
project(access_control)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
add_subdirectory(terminal-client)
add_subdirectory(app-server)
add_subdirectory(snippets)