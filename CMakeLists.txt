cmake_minimum_required(VERSION 3.6)
project(practie)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp compare_files.h compare_files.cpp find_duplicates.h find_duplicates.cpp synchronizing.h synchronizing.cpp)
add_executable(practie ${SOURCE_FILES})