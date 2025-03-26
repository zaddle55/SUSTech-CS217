#!/bin/bash
DATA_TYPES=("char" "short" "int" "float" "double")
SIZES=(100 1000 10000 100000 1000000 10000000 100000000)
OPT_LEVELS=("O0" "O1" "O2" "O3" "Ofast")
SRC_DIR="../src"
BUILD_DIR="../build"
CSV_DIR="../results_1"
mkdir -p $CSV_DIR

for opt in "${OPT_LEVELS[@]}"; do
    gcc -$opt -march=native $SRC_DIR/dotproduct.c -o $BUILD_DIR/dot_$opt -Wall

    # print header for .csv
    echo "Size,Data Type,Time" > $CSV_DIR/results_c_$opt.csv  
    # for dtype in {0..4}; do
    #     for size in "${SIZES[@]}"; do
    #         echo "Testing $opt ${DATA_TYPES[$dtype]} with size $size"
    #         ./dot_product_$opt $size $dtype >> results_c_$opt.txt
    #     done
    # done
    for dtype in {0..4}; do
        for size in "${SIZES[@]}"; do
            echo "Testing $opt ${DATA_TYPES[$dtype]} with size $size"
            echo -n "$size,${DATA_TYPES[$dtype]}," >> $CSV_DIR/results_c_$opt.csv
            $BUILD_DIR/dot_$opt $size $dtype >> $CSV_DIR/results_c_$opt.csv
        done
    done
done