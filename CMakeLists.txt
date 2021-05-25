cmake_minimum_required(VERSION 3.10)

project(LinuxSysMonitor LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_executable(process-monitor ${PROJECT_SOURCE_DIR}/src/Main.cpp)
target_include_directories(process-monitor PUBLIC "${PROJECT_BINARY_DIR}/src/")