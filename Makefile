CC = g++
CFLAGS = -g -Wall
LDFLAGS = `pkg-config --cflags --libs /usr/local/Cellar/opencv/3.4.3/lib/pkgconfig/opencv.pc`
OBJECTS = stereo.o
LINK_TARGET = stereo

all: $(LINK_TARGET)
	./$(LINK_TARGET)

stereo.o : src/stereo.cpp
	$(CC) -c $(CFLAGS) src/stereo.cpp

$(LINK_TARGET) : $(OBJECTS)
	$(CC) -o $(LINK_TARGET) $(LDFLAGS) $(OBJECTS)

clean:
	rm -f $(OBJECTS) $(LINK_TARGET)
	echo CLEAN DONE.
