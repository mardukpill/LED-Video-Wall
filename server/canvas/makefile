
CXX = g++ 

CXXFLAGS = -Wall -Wextra -O2 -std=c++17

#Include directories
INCLUDE_DIRS = -Iinclude -I$(EIGEN_DIR) -I$(OPENCV_DIR) -I/usr/local/include

#OpenCV library linking
LDFLAGS = `pkg-config --libs opencv4`


SRC = $(wildcard *.cpp)  
OBJ = $(SRC:.cpp=.o)     


TARGET = canvas      


all: $(TARGET)

#Rule to create the final executable by linking object files
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $^ -o $@ $(LDFLAGS)

#Rule to compile each .cpp file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@


clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
