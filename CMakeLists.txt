cmake_minimum_required(VERSION 3.22.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)
message(${CMAKE_CURRENT_BINARY_DIR})
message(${CMAKE_BINARY_DIR})

include_directories(includes)

project(AlphaCrane)

add_subdirectory(Logger)
