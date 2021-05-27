cmake_minimum_required(VERSION 3.10)

project(LinuxSysMonitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(process-monitor src/Main.cpp src/ProcessParser.cpp src/util.cpp) 
target_include_directories(process-monitor PUBLIC "${PROJECT_BINARY_DIR}/src/")