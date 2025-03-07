CC = g++
CFLAGS = -O3 -Wall -Wextra -pedantic -std=c++23 -pthread
GTFLAGS = -lgtest -lgtest_main -lpthread
PATH_TO_GTEST = /usr/lib 

# Директории
SRC_DIR = src
BUILD_DIR = build
EXPR_DIR = src/expressions

# Цели
all: $(BUILD_DIR) $(BUILD_DIR)/eval


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

test: $(BUILD_DIR)/tests
	@printf "Running tests...\n"
	@./$(BUILD_DIR)/tests

eval: $(BUILD_DIR)/eval
	@printf "Running eval...\n"
	@./$(BUILD_DIR)/eval

$(BUILD_DIR)/eval: $(BUILD_DIR)/expression.o $(BUILD_DIR)/eval.o
	@printf "Linking eval...\n"
	@$(CC) $(BUILD_DIR)/expression.o $(BUILD_DIR)/eval.o -o $(BUILD_DIR)/eval
	@printf "Linking eval is successful\n"

$(BUILD_DIR)/expression.o: $(EXPR_DIR)/expression.cpp $(EXPR_DIR)/expression.hpp
	@printf "Compiling Expression...\n"
	@$(CC) $(CFLAGS) -I $(EXPR_DIR) -c $(EXPR_DIR)/expression.cpp -o $(BUILD_DIR)/LongNum.o

$(BUILD_DIR)/eval.o: $(SRC_DIR)/eval.cpp $(EXPR_DIR)/expressions.hpp
	@printf "Compiling eval...\n"
	@$(CC) $(CFLAGS) -I $(EXPR_DIR) -c $(SRC_DIR)/eval.cpp -o $(BUILD_DIR)/eval.o

clean:
	@printf "Cleaning...\n"
	@rm -rf $(BUILD_DIR)
	@printf "Cleaning successful\n"

.PHONY: all test pi clean 