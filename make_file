# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# Executable name
TARGET = TMP110

# Source files
SRCS = tmp110.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Rule to build the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile the .cpp files into .o (object) files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
