CC = g++
CFLAGS = -O3 -Wall -Wextra -pedantic -std=c++23 -pthread
GTFLAGS = -lgtest -lgtest_main -lpthread
PATH_TO_GTEST = /usr/lib 

# Директории
SRC_DIR = src
BUILD_DIR = build
EXPR_DIR = src/expressions

# Цели
all: $(BUILD_DIR) $(BUILD_DIR)/tests


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

test: $(BUILD_DIR)/tests
	@printf "Running tests...\n"
	@./$(BUILD_DIR)/tests

$(BUILD_DIR)/tests: $(BUILD_DIR)/expression.o $(BUILD_DIR)/tests.o
	@printf "Linking tests...\n"
	@$(CC) $(BUILD_DIR)/expression.o $(BUILD_DIR)/tests.o -L $(PATH_TO_GTEST) $(GTFLAGS) -o $(BUILD_DIR)/tests
	@printf "Linking tests is successful\n"

$(BUILD_DIR)/expression.o: $(EXPR_DIR)/expression.cpp $(EXPR_DIR)/expression.hpp
	@printf "Compiling Expression...\n"
	@$(CC) $(CFLAGS) -I $(EXPR_DIR) -c $(EXPR_DIR)/expression.cpp -o $(BUILD_DIR)/expression.o

$(BUILD_DIR)/tests.o: $(SRC_DIR)/tests.cpp $(EXPR_DIR)/expression.hpp
	@printf "Compiling tests...\n"
	@$(CC) $(CFLAGS) -I $(EXPR_DIR) -c $(SRC_DIR)/tests.cpp -o $(BUILD_DIR)/tests.o

clean:
	@printf "Cleaning...\n"
	@rm -rf $(BUILD_DIR)
	@printf "Cleaning successful\n"

.PHONY: all test clean 