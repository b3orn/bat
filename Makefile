TARGET := bat

INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build


CCFLAGS := \
	-pedantic -fPIC \
	-Wall -Wextra \
	-Wno-padded \
	-Wno-poison-system-directories \
	-O0

LDFLAGS :=
LDLIBS :=
LIB_FLAGS :=

ifndef RELEASE
	CCFLAGS += -g -D BAT_DEBUG
endif

OS_ARCH := $(shell uname -m)
ifeq ($(OS_ARCH),x86_64)
	TARGET_ARCH := amd64
	CCFLAGS += -D BAT_ARCH_AMD64
endif

OS_NAME := $(shell uname -s)
ifeq ($(OS_NAME),Darwin)
	TARGET_OS := macos
	CC := clang
	CCFLAGS += -std=c11 -Weverything -D BAT_OS_MACOS
	LIB_FLAGS += -dynamiclib
	LIB_EXT := dylib
	ifeq ($(TARGET_ARCH),amd64)
		LDFLAGS += -arch x86_64
	endif
endif
ifeq ($(OS_NAME),Linux)
	TARGET_OS := linux
	CC := gcc
	CCFLAGS += -std=gnu11 -D BAT_OS_LINUX
	LDLIBS += -lm
	LIB_FLAGS += -shared
	LIB_EXT := so
	ifeq ($(TARGET_ARCH),amd64)
		LDFLAGS += -march=x86_64
	endif
endif

BUILD_VERSION := $(shell m4 build_version.m4; cat BUILD_VERSION)

CCFLAGS += \
	-D BAT_VERSION='"$(shell cat VERSION)"' \
	-D BAT_BUILD_DATE='"$(shell date)"' \
	-D BAT_BUILD_VERSION=$(BUILD_VERSION)


OBJ_DIR := $(BUILD_DIR)/$(TARGET_OS)-$(TARGET_ARCH)
BIN_DIR := $(OBJ_DIR)/bin
LIB_DIR := $(OBJ_DIR)/lib

INCLUDE_FILES := $(shell find $(INCLUDE_DIR) -name "*.h")
SRC_FILES := $(shell find $(SRC_DIR) -name "*.c")
OBJECT_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

BIN_TARGET := $(BIN_DIR)/$(TARGET)
LIB_TARGET := $(LIB_DIR)/lib$(TARGET).$(LIB_EXT)


.PHONY: all clean build-linux

all: $(BIN_TARGET) $(LIB_TARGET)

clean:
	-rm -rf $(BUILD_DIR)

build-linux:
	docker build -t $(TARGET) .
	docker run -it --mount type=bind,source=$(shell pwd),target=/mnt $(TARGET) make
	docker ps -a -f status=exited -f status=created -f ancestor=$(TARGET) -q | xargs docker rm


$(BIN_TARGET): $(OBJ_DIR)/$(TARGET).o $(LIB_TARGET)
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)


$(LIB_TARGET): $(filter-out $(OBJ_DIR)/$(TARGET).o,$(OBJECT_FILES))
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(LIB_FLAGS) -o $@ $^ $(LDLIBS)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES)
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<
