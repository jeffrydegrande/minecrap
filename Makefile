CC=g++
DEBUG=-D_DEBUG -g
CFLAGS=-Werror -fomit-frame-pointer -pipe -O2 -I. -Iinclude/ $(DEBUG) -O2 -Wno-error=switch
LDFLAGS=-lSDL -lGL -lglut -lGLU

all: minecrap

.cpp.o:
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $<

MINECRAP_SOURCES?=$(wildcard Minecrap/*.cpp)
MINECRAP_OBJS?=$(addprefix , $(MINECRAP_SOURCES:.cpp=.o))

minecrap: $(MINECRAP_OBJS)
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $(MINECRAP_OBJS) $(LDFLAGS)

clean: 
	rm Minecrap/*.o
