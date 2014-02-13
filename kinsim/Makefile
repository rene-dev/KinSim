CC=g++
CFLAGS=-g -c -Wall `pkg-config --cflags --static glfw3`
LDFLAGS=`pkg-config --static --libs glfw3`
SOURCES=main.cpp easygl.cpp stl.cpp path.c socket.c interpolator.c list.cpp gcode.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sim

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
