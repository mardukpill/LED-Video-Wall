# Config
CXX:=g++
CPPFLAGS:=
CXXFLAGS:=-O0 -g -Wall -std=c++17
INCFLAGS = -Iinclude -I/usr/include/opencv4 -I/usr/local/include
LDFLAGS:=-lyaml-cpp `pkg-config --libs opencv4`
SOURCES:=main tcp client config-parser canvas/canvas
OBJ_DIR:=obj

# Variables
CXX_FILES:=$(foreach src, $(SOURCES), $(src).cpp)
OBJ_FILES:=$(foreach src, $(SOURCES), $(OBJ_DIR)/$(src).o)

# Build
all: led-wall-server

led-wall-server: $(OBJ_FILES)
	$(CXX) $(INCFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_FILES): $(OBJ_DIR)/%.o: %.cpp obj
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

obj:
	mkdir obj

clean:
	rm $(OBJ_DIR)/*
