# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# project name
project(minibc)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

# add auxiliary files
file(GLOB DIR_SRCS
    "*.c"
    "*.h"
)

# add excluded files
list(REMOVE_ITEM DIR_SRCS
    "test_math.c"
)

# add executable
add_executable(minibc ${DIR_SRCS})
