# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2
LDFLAGS := -lSDL2

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := bin
TARGET := main

# Files
SRCFILES := $(wildcard $(SRCDIR)/*.cpp)
OBJFILES := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCFILES))

# Target: Build the final executable
$(TARGET): $(OBJFILES) | $(BINDIR)
	$(CXX) $(OBJFILES) -o $(BINDIR)/$(TARGET) $(LDFLAGS)

# Target: Compile the object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Create bin directory if it doesn't exist
$(BINDIR):
	mkdir -p $(BINDIR)

# Clean up build files
.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(BINDIR)
