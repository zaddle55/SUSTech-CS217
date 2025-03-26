#!/bin/bash
DATA_TYPES=("char" "short" "int" "float" "double")
SIZES=(100 1000 10000 100000 1000000 10000000 100000000)
OPT_LEVELS=("O0")
NUM_THREADS=(1 2 4 8 16 32)
SRC_DIR="../src"
BUILD_DIR="../build"
CSV_DIR="../results_2"
mkdir -p $CSV_DIR

for opt in "${OPT_LEVELS[@]}"; do
    gcc -$opt -march=native -fopenmp $SRC_DIR/dotproduct.c -o $BUILD_DIR/dot_$opt -Wall

    # print header for .csv
    echo "Num_Threads,Size,Data Type,Time" > $CSV_DIR/results_c_$opt.csv  
    for threads in "${NUM_THREADS[@]}"; do
        for dtype in {0..4}; do
            for size in "${SIZES[@]}"; do
                echo "Testing $opt ${DATA_TYPES[$dtype]} with size $size and $threads threads"
                echo -n "$threads,$size,${DATA_TYPES[$dtype]}," >> $CSV_DIR/results_c_$opt.csv
                $BUILD_DIR/dot_$opt $size $dtype $threads >> $CSV_DIR/results_c_$opt.csv
            done
        done
    done
done