OFILES = evolution.o read.o world-animation.o write.o xmalloc.o linked-list.o 
TARGET = evolution
CC = gcc
CFLAGS = -Wall -pedantic  -O2  $(shell sdl2-config --cflags) -Iinclude
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image
$(TARGET): $(OFILES)
    $(CC) $(OFILES) -o $@

evolution.o: evolution.h xmalloc.h 

clean:
    rm -f $(OFILES) $(TARGET)



# below this is the output of "gcc -MM":
