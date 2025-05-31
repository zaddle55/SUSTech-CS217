cmake_minimum_required(VERSION 3.28)
project(MyHalideProject)

# 设置 Halide 路径
set(Halide_ROOT "/home/zaddle/code/SUSTech-CS217/project5/Halide")  # 解压后的 halide 目录路径

# 查找 Halide
find_package(Halide REQUIRED)

# 创建可执行文件
add_executable(halide_task halide.cpp)

# 链接 Halide
target_link_libraries(halide_task PRIVATE Halide::Halide)