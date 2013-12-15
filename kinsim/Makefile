CC=g++
CFLAGS=-c -Wall `pkg-config --cflags --static glfw3`
LDFLAGS= `pkg-config --static --libs glfw3`
SOURCES=main.c easygl.cpp path.c socket.c interpolator.c list.cpp gcode.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sim

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(EXECUTABLE)
	rm *.o
