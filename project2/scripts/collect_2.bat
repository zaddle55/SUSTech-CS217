@echo off
setlocal enabledelayedexpansion

set DATA_TYPES=char short int float double
set SIZES=100 1000 10000 100000 1000000 10000000 100000000
set NUM_THREADS=1 2 4 8 16 32
set OPT_LEVELS=parallel
set SRC_DIR=.
set BUILD_DIR=.
set CSV_DIR=.\results_java
:: set OPTIM_OPTION=-XX:+UseG1GC -XX:MaxGCPauseMillis=200 -XX:+UseStringDeduplication

if not exist %CSV_DIR% mkdir %CSV_DIR%

:: Compile the Java file
javac %SRC_DIR%\Dotproduct.java

:: Run tests for each optimization mode
for %%o in (%OPT_LEVELS%) do (
    echo Num_Threads,Size,Data Type,Time > %CSV_DIR%\results_java_%%o.csv

    for %%t in (%NUM_THREADS%) do (
       for %%d in (%DATA_TYPES%) do (
            for %%s in (%SIZES%) do (
                echo Testing %%o %%d with size %%s
                echo|set /p="%%t,%%s,%%d," >> %CSV_DIR%\results_java_%%o.csv
                java -cp %BUILD_DIR% Dotproduct %%s %%d %%o %%t >> %CSV_DIR%\results_java_%%o.csv
            )
        )
    )
)

echo Testing complete. Results saved to %CSV_DIR%
endlocal