SRC_DIR = ./src
BUILD_DIR = ./build
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
TARGET = $(BUILD_DIR)/dot
INCLUDE = -I./inc
SIZE = 1000
TYPE = int
NUM_TRDS = 4
EXEC_OPT = $(SIZE) $(TYPE) $(NUM_TRDS)

CXX = gcc
CFALGS = -c -Wall -march=native -fopenmp
CXXFLAGES = $(CFALGS)
LDFLAGS = -fopenmp

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)
$(SRC_DIR)/%.c, $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CXXFLAGES) $< -o $@ $(INCLUDE)

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

gprof:
	$(CXX) $(LDFLAGS) -pg -o $(TARGET) $(OBJS)
	./$(TARGET) $(SIZE) $(TYPE)
	gprof -p $(TARGET) gmon.out > analysis.txt
	rm gmon.out
	mkdir -p ./analysis
	mv analysis.txt ./analysis/

asmO0:
	$(CXX) $(CXXFLAGES) -c -O0 $(SRC) -g -o $(BUILD_DIR)/dummy.o 
	objdump -s -d $(BUILD_DIR)/dummy.o > $(BUILD_DIR)/asmO0.s
	mkdir -p ./asm
	mv $(BUILD_DIR)/asmO0.s ./asm/
	rm $(BUILD_DIR)/dummy.o
	@echo "asmO0.s generated"

asmO2:
	$(CXX) $(CXXFLAGES) -c -O2 $(SRC) -g -o $(BUILD_DIR)/dummy.o 
	objdump -s -d $(BUILD_DIR)/dummy.o > $(BUILD_DIR)/asmO2.s
	mkdir -p ./asm
	mv $(BUILD_DIR)/asmO2.s ./asm/
	rm $(BUILD_DIR)/dummy.o
	@echo "asmO2.s generated"

asmO3:
	$(CXX) $(CXXFLAGES) -c -O3 $(SRC) -g -o $(BUILD_DIR)/dummy.o 
	objdump -s -d $(BUILD_DIR)/dummy.o > $(BUILD_DIR)/asmO3.s
	mkdir -p ./asm
	mv $(BUILD_DIR)/asmO3.s ./asm/
	rm $(BUILD_DIR)/dummy.o
	@echo "asmO3.s generated"

asmOfast:
	$(CXX) $(CXXFLAGES) -c -Ofast $(SRC) -g -o $(BUILD_DIR)/dummy.o 
	objdump -s -d $(BUILD_DIR)/dummy.o > $(BUILD_DIR)/asmOfast.s
	mkdir -p ./asm
	mv $(BUILD_DIR)/asmOfast.s ./asm/
	rm $(BUILD_DIR)/dummy.o
	@echo "asmOfast.s generated"

O2:
	$(CXX) $(CXXFLAGES) -O2 -c $(SRC) -o $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS)
	@echo "Optimized with O2"

O3:
	$(CXX) $(CXXFLAGES) -O3 -c $(SRC) -o $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS)
	@echo "Optimized with O3"

Ofast:
	$(CXX) $(CXXFLAGES) -Ofast -c $(SRC) -o $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS)
	@echo "Optimized with Ofast"

run:
	@echo "Running the program..."
	./$(TARGET) $(EXEC_OPT)
	@echo "Program finished running"
	@echo "Output file: $(TARGET)"
	@echo "Output file location: $(TARGET)"
	@echo "Output file size: $(shell du -h $(TARGET) | cut -f1)"

runclct_1:
	./collect_1.sh

cleancsv1:
	@echo "Cleaning up CSV files..."
	rm -f ./results_1/*.csv
	@echo "CSV files cleaned up"