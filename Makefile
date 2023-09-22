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

OBJECT_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(wildcard $(SRC_DIR)/*.c))
INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/%.h)

TARGET = bat


.PHONY: all clean


all: $(BUILD_DIR)/$(TARGET)


clean:
	-rm -rf $(BUILD_DIR)


$(BUILD_DIR)/$(TARGET): $(OBJECT_FILES)
	$(CC) $(LDFLAGS) -o $@ $^


$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<
