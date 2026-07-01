CC=clang
CFLAGS=-std=c23 -Wall -Werror -Wpedantic -g -fsanitize=address
IFLAGS=-Isrc -Iutil
LFLAGS=-lasan

SRC_DIR=src
UTIL_DIR=util
BUILD_DIR=build



$(BUILD_DIR)/tokenizer: $(BUILD_DIR)/tokenizer.o $(BUILD_DIR)/list.o $(BUILD_DIR)/token.o
	$(CC) $(LFLAGS) $^ -o $@


$(BUILD_DIR)/token.o: $(SRC_DIR)/token.c $(SRC_DIR)/token.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(BUILD_DIR)/tokenizer.o: $(SRC_DIR)/tokenizer.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@


$(BUILD_DIR)/list.o: $(UTIL_DIR)/list.c $(UTIL_DIR)/list.h
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@
