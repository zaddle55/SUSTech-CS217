g++ src/halide.cpp -g -std=c++17 \
    -I./Halide/include \
    -L./Halide/lib \
    -lHalide -lpthread -ldl \
    -march=native \
    -O3 -DNDEBUG \
    -o ./build/halide_task
LD_LIBRARY_PATH=./Halide/lib:$LD_LIBRARY_PATH ./build/halide_task