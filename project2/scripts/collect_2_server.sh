#!/bin/bash

DATA_TYPES="char short int float double"
SIZES="100 1000 10000 100000 1000000 10000000 100000000"
NUM_THREADS="1 2 4 8 16 32"
OPT_LEVELS="parallel"
SRC_DIR="../src"
BUILD_DIR="../build"
CSV_DIR="../results_java"
# OPTIM_OPTION="-XX:+UseG1GC -XX:MaxGCPauseMillis=200 -XX:+UseStringDeduplication"
mkdir -p ${CSV_DIR}

# javac ${SRC_DIR}/Dotproduct.java -d ${BUILD_DIR}

for o in ${OPT_LEVELS}; do
    echo "Num_Threads,Size,Data Type,Time" > ${CSV_DIR}/results_java_${o}.csv

    for t in ${NUM_THREADS}; do
        for d in ${DATA_TYPES}; do
            for s in ${SIZES}; do
                echo "Testing ${o} ${d} with size ${s}"
                echo -n "${t},${s},${d}," >> ${CSV_DIR}/results_java_${o}.csv
                java ${SRC_DIR}/Dotproduct.java ${s} ${d} ${o} ${t} >> ${CSV_DIR}/results_java_${o}.csv
            done
        done
    done
done

echo "Testing complete. Results saved to ${CSV_DIR}"