cmake_minimum_required(VERSION 3.15)

# set the project name
project(Routines)
enable_language(C ASM)
# add the executable
add_executable(a simple_context.S main.cpp routines.hpp routines.cpp simple_context.hpp)