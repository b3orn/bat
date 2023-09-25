TARGET := bat

INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build
DEPS_DIR := libs
DEPS_INCLUDE_DIRS := -I$(DEPS_DIR)/clap/include


AR := ar

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
else ifeq ($(OS_ARCH),aarch64)
	TARGET_ARCH := arm64
	CCFLAGS += -D BAT_ARCH_ARM64
else ifeq ($(OS_ARCH),arm64)
	TARGET_ARCH := arm64
	CCFLAGS += -D BAT_ARCH_ARM64
else
$(error "Unsupported architecture ${OS_ARCH}")
endif

OS_NAME := $(shell uname -s)
ifeq ($(OS_NAME),Darwin)
	TARGET_OS := macos
	CC := clang
	CCFLAGS += -std=c11 -Weverything -D BAT_OS_POSIX -D BAT_OS_MACOS
	LDFLAGS += -arch $(OS_ARCH)
	LIB_FLAGS += -dynamiclib
	LIB_EXT := dylib
else ifeq ($(OS_NAME),Linux)
	TARGET_OS := linux
	CC := gcc
	CCFLAGS += -std=gnu11 -D BAT_OS_POSIX -D BAT_OS_LINUX
	LDFLAGS += -march=$(OS_ARCH)
	LDLIBS += -lm
	LIB_FLAGS += -shared
	LIB_EXT := so
else
$(error "Unsupported operating system ${OS_NAME}")
endif

BUILD_VERSION := $(shell m4 build_version.m4 && cat BUILD_VERSION)
ifeq ($(BUILD_VERSION),)
$(error "No build version defined")
endif

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
LIB_OBJECT_FILES := $(filter-out $(OBJ_DIR)/$(TARGET).o,$(OBJECT_FILES))

BIN_TARGET := $(BIN_DIR)/$(TARGET)
LIB_TARGET := $(LIB_DIR)/$(TARGET).clap
STATIC_LIB_TARGET := $(LIB_DIR)/lib$(TARGET).a


.PHONY: all clean wrapper build-linux

all: $(LIB_TARGET)

clean:
	-rm -rf $(BUILD_DIR)

wrapper:
	mkdir -p $(OBJ_DIR)/wrapper
	cmake \
		-S $(DEPS_DIR)/clap-wrapper \
		-B $(OBJ_DIR)/wrapper \
		-DCLAP_SDK_ROOT=$(shell realpath $(DEPS_DIR)/clap) \
		-DCLAP_WRAPPER_DOWNLOAD_DEPENDENCIES=TRUE \
		-DCLAP_WRAPPER_BUILD_AUV2=TRUE \
		-DCLAP_WRAPPER_OUTPUT_NAME=$(TARGET)
	cmake --build $(OBJ_DIR)/wrapper
	-rm $(OBJ_DIR)/wrapper/cpm
	-rm $(OBJ_DIR)/wrapper/CMakeFiles
	-mv $(OBJ_DIR)/wrapper/$(TARGET).vst3 $(OBJ_DIR)
	-mv $(OBJ_DIR)/wrapper/$(TARGET).component $(OBJ_DIR)


build-linux:
	docker build -t $(TARGET) .
	docker run -it --mount type=bind,source=$(shell pwd),target=/mnt $(TARGET) make
	docker ps -a -f status=exited -f status=created -f ancestor=$(TARGET) -q | xargs docker rm


$(BIN_TARGET): $(OBJ_DIR)/$(TARGET).o $(STATIC_LIB_TARGET)
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)


$(LIB_TARGET): $(LIB_OBJECT_FILES)
	mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $(LIB_FLAGS) -o $@ $^ $(LDLIBS)


$(STATIC_LIB_TARGET): $(LIB_OBJECT_FILES)
	mkdir -p $(dir $@)
	$(AR) rcs $@ $^


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES)
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) $(DEPS_INCLUDE_DIRS) -o $@ -c $<
