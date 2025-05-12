SRC_DIR = ./src
BUILD_DIR = ./build
TEST_DIR = ./tests
INC = ./inc
IMG_DIR = ./img

TESTS = $(wildcard $(TEST_DIR)/*.c)

CXX = gcc
CFALGS = -Wall -O3 -march=native
CXXFLAGES = $(CFALGS)

teststream:
	$(CXX) $(CXXFLAGES) $(TEST_DIR)/test_stream.c $(SRC_DIR)/stream.c -o $(BUILD_DIR)/test_stream -lm -g
	$(BUILD_DIR)/test_stream
testbmp:
	$(CXX) $(CXXFLAGES) $(TEST_DIR)/test_bmp.c $(SRC_DIR)/bmp.c $(SRC_DIR)/stream.c $(SRC_DIR)/pmat.c $(SRC_DIR)/color.c -o $(BUILD_DIR)/test_bmp -lm
	$(BUILD_DIR)/test_bmp
testconv:
	$(CXX) $(CXXFLAGES) $(TEST_DIR)/test_conv.c $(SRC_DIR)/bmp.c $(SRC_DIR)/stream.c $(SRC_DIR)/pmat.c $(SRC_DIR)/color.c -o $(BUILD_DIR)/test_conv -lm
	$(BUILD_DIR)/test_conv

cimage:
	# $(CXX) $(CXXFLAGES) $(SRC_DIR)/main.c $(SRC_DIR)/bmp.c $(SRC_DIR)/stream.c $(SRC_DIR)/pmat.c $(SRC_DIR)/color.c -o $(IMG_DIR)/cimage -lm
	$(CXX) $(CXXFLAGES) $(SRC_DIR)/submission_ver.c -o $(IMG_DIR)/cimage -lm