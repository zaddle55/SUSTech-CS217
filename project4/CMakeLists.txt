cmake_minimum_required(VERSION 3.10)
project(mcv_project VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Base compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -std=c++17")
set(DEBUG_FLAGS "-DMCV_DBG")
set(BENCH_FLAGS "-DMCV_BENCH")
set(OMP_FLAGS "-fopenmp")

add_library(mcv SHARED mcv.cpp)
set_target_properties(mcv PROPERTIES 
    POSITION_INDEPENDENT_CODE ON
    VERSION 1.0
    SOVERSION 1)

# Main executable
add_executable(example example.cpp)
target_link_libraries(example mcv)

# Custom targets
add_custom_target(clean_all
    COMMAND ${CMAKE_MAKE_PROGRAM} clean
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaned all build files"
)

# Debug target
add_custom_target(debug
    COMMAND ${CMAKE_COMMAND} -E echo "Building with debug flags..."
    COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} ${DEBUG_FLAGS} -o debug_example example.cpp mcv.cpp
    COMMAND ${CMAKE_COMMAND} -E echo "Debug build complete: debug_example"
)

# Bench target
add_custom_target(bench
    COMMAND ${CMAKE_COMMAND} -E echo "Building with benchmark flags..."
    COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} ${BENCH_FLAGS} -o bench_example example.cpp mcv.cpp
    COMMAND ${CMAKE_COMMAND} -E echo "Benchmark build complete: bench_example"
)

# OpenMP target
add_custom_target(omp
    COMMAND ${CMAKE_COMMAND} -E echo "Building with OpenMP flags..."
    COMMAND ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} ${OMP_FLAGS} -o omp_example example.cpp mcv.cpp
    COMMAND ${CMAKE_COMMAND} -E echo "OpenMP build complete: omp_example"
)

# Example target that creates img directory and downloads image
add_custom_target(assets
    COMMAND ${CMAKE_COMMAND} -E echo "Setting up example environment..."
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/img
    COMMAND ${CMAKE_COMMAND} -E echo "Created img directory"
    COMMAND curl -o ${CMAKE_CURRENT_SOURCE_DIR}/img/sustech.jpg https://www.sustech.edu.cn/uploads/full/2020/08/25165059_71675.jpg
    COMMAND ${CMAKE_COMMAND} -E echo "Downloaded image to img/sustech.jpg"
)