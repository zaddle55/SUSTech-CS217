# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# project name
project(p1)

# C++ standard
set(CMAKE_CXX_STANDARD 11)

# add auxiliary files
aux_source_directory(./src DIR_SRCS)

# add include directories
include_directories(./inc)

# add executable
add_executable(main ${DIR_SRCS})