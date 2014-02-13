CC=g++
CFLAGS=-g -c -Wall -I../kinsim `pkg-config --cflags --static glfw3` `wx-config --cppflags`
LDFLAGS=`wx-config --libs gl`
SOURCES=main.cpp ../kinsim/easygl.cpp ../kinsim/stl.cpp ../kinsim/path.c ../kinsim/socket.c ../kinsim/interpolator.c ../kinsim/list.cpp ../kinsim/gcode.cpp 
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sim

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE).app

bundle: $(EXECUTABLE)
	mkdir -p $(EXECUTABLE).app/Contents/MacOS/
	mv $(EXECUTABLE) $(EXECUTABLE).app/Contents/MacOS/