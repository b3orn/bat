CC = clang

CCFLAGS = \
	-Wall -Wextra -Weverything -std=c11 -pedantic -fPIC \
	-Wno-padded \
	-Wno-poison-system-directories \
	-O0 -g

LDFLAGS = -arch x86_64

INCLUDE_DIR = include
SRC_DIR = src
BUILD_DIR = build

INCLUDE_FILES = $(shell find $(INCLUDE_DIR) -name "*.h")
SRC_FILES = $(shell find $(SRC_DIR) -name "*.c")
OBJECT_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

TARGET = bat


.PHONY: all clean


all: $(BUILD_DIR)/$(TARGET)


clean:
	-rm -rf $(BUILD_DIR)


$(BUILD_DIR)/$(TARGET): $(OBJECT_FILES)
	$(CC) $(LDFLAGS) -o $@ $^


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES)
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<
