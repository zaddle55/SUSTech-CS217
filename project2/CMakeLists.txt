# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# project name
project(project2)

# C standard
set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_FLAGS "-O3")

# C++ standard
set(CMAKE_CXX_STANDARD 11)

# add auxiliary files
aux_source_directory(. DIR_SRCS)

# add executable
add_executable(project2 ${DIR_SRCS})