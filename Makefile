CC=g++
DEBUG=-D_DEBUG -g
CFLAGS=-Werror -fomit-frame-pointer -pipe -O2 -I. -Iinclude/ $(DEBUG) -O2 -Wno-error=switch

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
LDFLAGS =-L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa -framework OpenGL -framework GLUT
else
LDFLAGS=-lSDL -lGL -lglut -lGLU
endif

all: mc

.cpp.o:
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $<

MINECRAP_SOURCES?=$(wildcard Minecrap/*.cpp)
MINECRAP_OBJS?=$(addprefix , $(MINECRAP_SOURCES:.cpp=.o))

mc: $(MINECRAP_OBJS)
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $(MINECRAP_OBJS) $(LDFLAGS)

clean: 
	rm Minecrap/*.o mc
