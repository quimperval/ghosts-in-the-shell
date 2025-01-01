CXX := clang++

CXXFLAGS := -Iutils

BUILD_DIR := build
SRC_DIR := .
UTIL_DIR := utils

UTIL_SRCS := $(wildcard $(UTIL_DIR)/*.cpp)
UTIL_OBJS := $(patsubst $(UTIL_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(UTIL_SRCS))
MAIN_SRCS := main.cpp
MAIN_OBJ := main.o

TARGET := game

LIBS := -lncurses

LIB_DIRS := 

ALL := $(TARGET)

# Linking the final executable
$(TARGET): $(UTIL_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB_DIRS) $(LIBS)

# Compile utility classes
$(BUILD_DIR)/%.o: $(UTIL_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main.cpp
$(BUILD_DIR)/main.o: $(MAIN_SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
