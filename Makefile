CC=g++
DEBUG=-D_DEBUG -g
INCLUDES=-I. -Iinclude/ -Ivendor/include
# CFLAGS=-Wall -Werror -fomit-frame-pointer -pipe -O2 $(INCLUDES)
CFLAGS=-Wall -Werror $(INCLUDES)

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
CFLAGS+=-DHAVE_APPLE_OPENGL_FRAMEWORK
LDFLAGS =-L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa -framework OpenGL
VENDOR_OBJS?=./vendor/lib/osx/libcvars.a
else
LDFLAGS=-lSDL -lGL -lGLU
VENDOR_OBJS?=./vendor/lib/linux/libcvars.a
endif

all: mc

.cpp.o:
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $<

MINECRAP_SOURCES?=$(wildcard src/*.cpp)
MINECRAP_OBJS?=$(addprefix , $(MINECRAP_SOURCES:.cpp=.o))


mc: $(MINECRAP_OBJS)
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $(MINECRAP_OBJS) $(VENDOR_OBJS) $(LDFLAGS)

clean: 
	rm src/*.o mc
