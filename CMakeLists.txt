cmake_minimum_required(VERSION 3.10)

project(AutomaticDifferentiation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(automatic-differentiation
    src/main.cpp
    src/ADDual.cpp
    src/ADBackward.cpp
    src/ADForward.cpp
)

target_include_directories(automatic-differentiation PRIVATE src include)