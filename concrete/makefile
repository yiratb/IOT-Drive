# Variables
CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -Iinc -I../framework/inc -g -fPIC
LDFLAGS = -pthread -ldl -lrt
SRC_DIRS = ../framework/src src
INC_DIRS = ../framework/inc inc
TEST_DIR = test
BIN_DIR = bin/debug
OBJ_DIR = obj

TARGET = final_project
TEST_TARGET1 = final_project_test
TEST_TARGETS = minion_test1 minion_test2 minion_test3

# Find all source files and corresponding object files
SRCS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJS = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))
TEST_SRCS1 = $(wildcard $(TEST_DIR)/final_project_test.cpp)
TEST_OBJS1 = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(TEST_SRCS1)))
TEST_SRCS2 = $(wildcard $(TEST_DIR)/minion_test1.cpp $(TEST_DIR)/minion_test2.cpp $(TEST_DIR)/minion_test3.cpp)
TEST_OBJS2 = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(TEST_SRCS2)))

# Create directories if they don't exist
$(shell mkdir -p $(BIN_DIR) $(OBJ_DIR))

# Default target
.PHONY: all
all: $(BIN_DIR)/$(TEST_TARGET1).out $(foreach target, $(TEST_TARGETS), $(BIN_DIR)/$(target).out)

# Build first test target
$(BIN_DIR)/$(TEST_TARGET1).out: $(OBJS) $(TEST_OBJS1)
	$(CC) -o $@ $(filter-out $(OBJ_DIR)/minion_test1.o $(OBJ_DIR)/minion_test2.o $(OBJ_DIR)/minion_test3.o, $(OBJS) $(TEST_OBJS1)) $(LDFLAGS)
	@echo "$@ Compiled Successfully!"

# Build other test targets
$(foreach target, $(TEST_TARGETS), $(eval $(BIN_DIR)/$(target).out: $(OBJS) $(OBJ_DIR)/$(target).o))

$(BIN_DIR)/minion_test1.out: $(OBJS) $(OBJ_DIR)/minion_test1.o
	$(CC) -o $@ $(filter-out $(OBJ_DIR)/final_project_test.o $(OBJ_DIR)/minion_test2.o $(OBJ_DIR)/minion_test3.o, $(OBJS) $(OBJ_DIR)/minion_test1.o) $(LDFLAGS)
	@echo "$@ Compiled Successfully!"

$(BIN_DIR)/minion_test2.out: $(OBJS) $(OBJ_DIR)/minion_test2.o
	$(CC) -o $@ $(filter-out $(OBJ_DIR)/final_project_test.o $(OBJ_DIR)/minion_test1.o $(OBJ_DIR)/minion_test3.o, $(OBJS) $(OBJ_DIR)/minion_test2.o) $(LDFLAGS)
	@echo "$@ Compiled Successfully!"

$(BIN_DIR)/minion_test3.out: $(OBJS) $(OBJ_DIR)/minion_test3.o
	$(CC) -o $@ $(filter-out $(OBJ_DIR)/final_project_test.o $(OBJ_DIR)/minion_test1.o $(OBJ_DIR)/minion_test2.o, $(OBJS) $(OBJ_DIR)/minion_test3.o) $(LDFLAGS)
	@echo "$@ Compiled Successfully!"

# Compile source files into object files
$(OBJ_DIR)/%.o: ../framework/src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test files into object files
$(OBJ_DIR)/final_project_test.o: $(TEST_DIR)/final_project_test.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/minion_test1.o: $(TEST_DIR)/minion_test1.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/minion_test2.o: $(TEST_DIR)/minion_test2.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/minion_test3.o: $(TEST_DIR)/minion_test3.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*.out
	@echo "Cleaned up!"
