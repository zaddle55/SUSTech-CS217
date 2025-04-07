cmake_minimum_required(VERSION 3.12)

project(lab07_p1)

aux_source_directory(. DIR_SRCS)

include_directories(lib)

add_subdirectory(lib)

add_executable(main ${DIR_SRCS})

# target_link_libraries(main libvabs_static)
target_link_libraries(main libvabs)