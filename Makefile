CC = clang
LD = ld
AR = ar

CCFLAGS = \
	-Wall -Wextra -Weverything -std=c11 -pedantic -fPIC \
	-Wno-padded \
	-Wno-poison-system-directories \
	-O0 -g

LDFLAGS = -macosx_version_min 13.0 \
	-arch x86_64 \
	-L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib \
	-lSystem

INCLUDE_DIR = include
SRC_DIR = src
OBJECT_DIR = $(BUILD_DIR)

OBJECT_FILES = $(shell find $(SRC_DIR) -name '*.c' -exec echo {} \; | sed 's/\.c/\.o/')

INCLUDE_FILES = $(shell find $(INCLUDE_DIR) -name '*.h')

TARGET=bat


.PHONY: all clean


all: $(TARGET)  # lib$(TARGET).dylib lib$(TARGET).a


clean:
	-rm $(OBJECT_FILES)
	#-rm lib$(TARGET).dylib
	#-rm lib$(TARGET).a
	-rm $(TARGET)


#lib$(TARGET).dylib: $(filter-out $(SRC_DIR)/$(TARGET).o,$(OBJECT_FILES))
#	$(LD) $(LDFLAGS) -dylib -o $@ $+


#lib$(TARGET).a: $(filter-out $(SRC_DIR)/$(TARGET).o,$(OBJECT_FILES))
#	$(AR) rcs $@ $+


#$(TARGET): libbat.dylib $(SRC_DIR)/$(TARGET).o
#	$(LD) $(LDFLAGS) -execute -L. -l$(TARGET) -o $@ $(SRC_DIR)/$(TARGET).o


$(TARGET): $(OBJECT_FILES)
	$(CC) -o $@ $+


$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_FILES)
	$(CC) $(CCFLAGS) -I$(INCLUDE_DIR) -o $@ -c $<
