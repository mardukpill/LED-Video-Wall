# Config
CXX:=g++
CPPFLAGS:=
CXXFLAGS:=-O0 -g -Wall
LDFLAGS:=
SOURCES:=main
OBJ_DIR:=obj

# Variables
CXX_FILES:=$(foreach src, $(SOURCES), $(src).cpp)
OBJ_FILES:=$(foreach src, $(SOURCES), $(OBJ_DIR)/$(src).o)

# Build
all: led-wall-server

led-wall-server: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_FILES): $(OBJ_DIR)/%.o: %.cpp obj
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

obj:
	mkdir obj

clean:
	rm $(OBJ_DIR)/*
