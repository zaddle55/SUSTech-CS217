# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# project name
project(minibc)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -lm")

LINK_LIBRARIES(m)

# add auxiliary files
aux_source_directory(./src DIR_SRCS)

# add test files
aux_source_directory(./test DIR_TEST_SRCS)

# add executable
add_executable(minibc ${DIR_SRCS})
