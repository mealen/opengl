CXX = g++
# uygulamanin derlenebilmesi icin asagidaki degerleri degistirmeniz gerekebilir 


LDFLAGS=-L/usr/lib/nvidia-331/ -L/usr/local/lib/

ifeq ($(OS),Windows_NT)
    LIBS=-lglfw3 -lGLEW32 -lopengl32  -lgdi32
else
	LIBS=-lGL -lGLU -lglfw3 -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi
endif

INCLUDES=-I/usr/include

#bu noktadan sonra degisiklik yapmaniz gerekmemeli 
CXXFLAGS=-Wall -ggdb -c 
EXE=ornekLatest
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
