cmake_minimum_required(VERSION 3.12)
project(cDB)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES "src/*cpp")

add_library(cDBParser ${SOURCES})

add_subdirectory(tests)

add_executable(cDBMainTest ${SOURCES})
