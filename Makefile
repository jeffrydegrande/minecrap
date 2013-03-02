CC=g++
DEBUG=-D_DEBUG -g
INCLUDES=-I. -Ivendor/include
# CFLAGS=-Wall -Werror -fomit-frame-pointer -pipe -O2 $(INCLUDES)
#
FEATURES=-DSUPPORT_GLCONSOLE

WARNINGS=-pedantic -Werror -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
WARNINGS=-Wall -Werror -Wextra

CFLAGS=$(WARNINGS) $(INCLUDES) $(FEATURES)

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
	@echo "$(CC) -o $@"
	@$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $<

MINECRAP_SOURCES?=$(wildcard src/*.cpp)
MINECRAP_OBJS?=$(addprefix , $(MINECRAP_SOURCES:.cpp=.o))


mc: $(MINECRAP_OBJS)
	@$(CC) $(CFLAGS) $(DEBUG) -o $@ $(MINECRAP_OBJS) $(VENDOR_OBJS) $(LDFLAGS)

clean: 
	rm src/*.o mc
