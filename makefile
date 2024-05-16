CC = gcc
CFLAGS = -Wall -Wextra -I./inc/ -fms-extensions
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
SRC_FILES += main.c
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
TARGET = my_program
all: $(BIN_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -fPIC

$(BIN_DIR)/$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ -fPIC

clean:
	rm -f bin/* obj/*

.PHONY: build clean

build: all