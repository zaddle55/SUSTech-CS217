# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# project name
project(p3)

# C++ standard
set(CMAKE_CXX_STANDARD 11)

# add auxiliary files
aux_source_directory(. DIR_SRCS)

# add executable
add_executable(p3 ${DIR_SRCS})