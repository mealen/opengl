CXX = g++
# Update these paths to match your installation
# You may also need to update the linker option rpath, which sets where to look for
# the GLFW libraries at runtime to match your install
#GLFW_INCLUDE = -I/usr/include

CXXFLAGS=-Wall -ggdb -c 
LDFLAGS=-L/usr/lib/nvidia-331/ -L/usr/local/lib/
LIBS=-lGL -lGLU -lglfw -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
INCLUDES=-I/usr/include
EXE=Tutorial6
BINARY_PATH=bin/
SOURCE_PATH=src/

all: $(BINARY_PATH)$(EXE)

$(BINARY_PATH)$(EXE): $(BINARY_PATH)GlfwHelper.o $(BINARY_PATH)GlHelper.o $(BINARY_PATH)main.o 
	$(CXX) $^ $(LDFLAGS) -o $@ $(LIBS)

$(BINARY_PATH)main.o: $(SOURCE_PATH)main.cpp 
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)GlfwHelper.o: $(SOURCE_PATH)GlfwHelper.cpp $(SOURCE_PATH)GlfwHelper.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BINARY_PATH)GlHelper.o: $(SOURCE_PATH)GlHelper.cpp $(SOURCE_PATH)GlHelper.h
	mkdir -p $(BINARY_PATH)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -rf $(BINARY_PATH)/*.o $(BINARY_PATH)/$(EXE)
