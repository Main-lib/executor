# Simple Makefile for iOS Executor
# This can be used to easily build the project without CMake

# Configuration
CXX = clang++
SDK = $(shell xcrun --sdk iphoneos --show-sdk-path)
ARCHS = arm64
PLATFORM = iphoneos
MIN_VERSION = 15.0

# Directories
SRCDIR = source/cpp
BUILDDIR = build
OUTDIR = $(BUILDDIR)/lib

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I./source -I./source/cpp -I./VM/include
CXXFLAGS += -isysroot $(SDK) -arch $(ARCHS) -mios-version-min=$(MIN_VERSION)
CXXFLAGS += -DPLATFORM_IOS=1
LDFLAGS = -dynamiclib -isysroot $(SDK) -arch $(ARCHS)

# Source files (excluding Objective-C++ files)
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

# Main target
all: $(OUTDIR)/libexecutor.dylib

# Create output directories
$(OUTDIR):
mkdir -p $(OUTDIR)

$(BUILDDIR):
mkdir -p $(BUILDDIR)

# Compile C++ files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
$(CXX) $(CXXFLAGS) -c $< -o 

# Link the library
$(OUTDIR)/libexecutor.dylib: $(OBJECTS) | $(OUTDIR)
$(CXX) $(LDFLAGS) $(OBJECTS) -o 

# Clean build files
clean:
rm -rf $(BUILDDIR)

.PHONY: all clean
