CC = g++
CFLAGS = -O3 -Wall -Wextra -pedantic -std=c++23 -pthread
GTFLAGS = -lgtest -lgtest_main -lpthread
PATH_TO_GTEST = /usr/lib 

# Директории
SRC_DIR = src
BUILD_DIR = build
EXPR_DIR = src/expressions
PARSER_DIR = src/parser
# Цели
all: $(BUILD_DIR) $(BUILD_DIR)/tests $(BUILD_DIR)/differentiator


$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

test: $(BUILD_DIR)/tests
	@printf "Running tests...\n"
	@./$(BUILD_DIR)/tests

differentiator: $(BUILD_DIR)/differentiator | $(BUILD_DIR)
	$(BUILD_DIR)/differentiator $(ARGS)

$(BUILD_DIR)/tests: $(BUILD_DIR)/expression.o $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/tests.o
	@printf "Linking tests...\n"
	@$(CC) $(BUILD_DIR)/expression.o $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/tests.o -L $(PATH_TO_GTEST) $(GTFLAGS) -o $(BUILD_DIR)/tests
	@printf "Linking tests is successful\n"

$(BUILD_DIR)/differentiator: $(BUILD_DIR)/expression.o $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/differentiator.o
	@printf "Linking differentiator...\n"
	@$(CC) $(BUILD_DIR)/expression.o $(BUILD_DIR)/lexer.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/differentiator.o -o $(BUILD_DIR)/differentiator
	@printf "Linking differentiator is successful\n"


$(BUILD_DIR)/expression.o: $(EXPR_DIR)/expression.cpp $(EXPR_DIR)/expression.hpp
	@printf "Compiling Expression...\n"
	@$(CC) $(CFLAGS) -I $(EXPR_DIR) -c $(EXPR_DIR)/expression.cpp -o $(BUILD_DIR)/expression.o

$(BUILD_DIR)/tests.o: $(SRC_DIR)/tests.cpp $(EXPR_DIR)/expression.hpp $(PARSER_DIR)/lexer.hpp $(PARSER_DIR)/parser.hpp
	@printf "Compiling tests...\n"
	@$(CC) $(CFLAGS) -I $(EXPR_DIR) -I $(PARSER_DIR) -c $(SRC_DIR)/tests.cpp -o $(BUILD_DIR)/tests.o

$(BUILD_DIR)/lexer.o: $(PARSER_DIR)/lexer.cpp $(PARSER_DIR)/lexer.hpp
	@printf "Compiling Lexer...\n"
	@$(CC) $(CFLAGS) -I $(PARSER_DIR) -c $(PARSER_DIR)/lexer.cpp -o $(BUILD_DIR)/lexer.o

$(BUILD_DIR)/parser.o: $(PARSER_DIR)/parser.cpp $(PARSER_DIR)/parser.hpp
	@printf "Compiling Parser...\n"
	@$(CC) $(CFLAGS) -I $(PARSER_DIR) -c $(PARSER_DIR)/parser.cpp -o $(BUILD_DIR)/parser.o

$(BUILD_DIR)/differentiator.o: $(SRC_DIR)/differentiator.cpp $(EXPR_DIR)/expression.hpp $(PARSER_DIR)/lexer.hpp $(PARSER_DIR)/parser.hpp
	@printf "Compiling Parser...\n"
	@$(CC) $(CFLAGS) -I $(PARSER_DIR) -c $(SRC_DIR)/differentiator.cpp -o $(BUILD_DIR)/differentiator.o

clean:
	@printf "Cleaning...\n"
	@rm -rf $(BUILD_DIR)
	@printf "Cleaning successful\n"

.PHONY: all test differentiator clean