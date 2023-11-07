cmake_minimum_required(VERSION 3.13)  # CMake version check
project(ch11_7)               # Create project "simple_example"

# Add main.cpp file of the project root directory as a source file
set(SOURCE_FILES
        list_util_test.c
        list_util.c
        linked_list.c
        maybe_int.c
        array_int64.c
        int_util.h
        int_util.c)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(ch11_7 ${SOURCE_FILES})
