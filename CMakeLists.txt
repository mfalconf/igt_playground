cmake_minimum_required(VERSION 3.14)
project(LibreriasPruebas VERSION 1.0 LANGUAGES CXX C)

# Global configuration: Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Instruct CMake to process the following folders
add_subdirectory(poco/demo1)
add_subdirectory(poco/demo2)
add_subdirectory(poco/demo_plugin)
add_subdirectory(sqlite)
add_subdirectory(gsoap/demo1)
add_subdirectory(gsoap/demo2)
add_subdirectory(spread)

add_subdirectory(drive_monitor_demo)
