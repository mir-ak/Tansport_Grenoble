
CC = gcc

CFLAGS := -Wall -g

CPPFLAGS := -Iinclude

EXE := Grenoble

SRC_DIR := src
OBJ_DIR := obj

SRC = $(wildcard $(SRC_DIR)/*.c)

OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

RM := rm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) $(OBJ)
	rmdir $(OBJ_DIR)