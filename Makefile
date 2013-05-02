CC=g++
DEBUG=-D_DEBUG -g
INCLUDES=-I. -Ivendor/include
# CFLAGS=-Wall -Werror -fomit-frame-pointer -pipe -O2 $(INCLUDES)

FEATURES=# -DSUPPORT_GLCONSOLE

WARNINGS=-pedantic -Werror -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused
WARNINGS=-Wall -Werror -Wextra

CFLAGS=$(WARNINGS) $(INCLUDES) $(FEATURES)

OS=$(shell uname -s)
ifeq ($(OS),Darwin)
CFLAGS+=-DHAVE_APPLE_OPENGL_FRAMEWORK
LIB_CVARS=./vendor/lib/osx/libcvars.a
LDFLAGS =-L/usr/local/lib -lSDLmain -lSDL2 -Wl,-framework,Cocoa -framework OpenGL
else
LIB_CVARS?=./vendor/lib/linux/libcvars.a
LDFLAGS=-lSDL -lGL -lGLU -lIL -lILU
endif

all: mc tests

.cpp.o:
	@echo "Compiling $@"
	@$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $<

LIB_MINECRAP_SOURCES= src/AABox.cpp \
		      src/Camera.cpp \
		      src/Chunk.cpp \
		      src/Color.cpp \
		      src/Console.cpp \
		      src/Crosshair.cpp \
		      src/Engine.cpp \
		      src/File.cpp \
		      src/Frustum.cpp \
		      src/Input.cpp \
		      src/Matrix.cpp \
		      src/MatrixStack.cpp \
		      src/Mesh.cpp \
		      src/OSD.cpp \
		      src/Plane.cpp \
		      src/Player.cpp \
		      src/Shader.cpp \
		      src/Sun.cpp \
		      src/Text.cpp \
		      src/Vec.cpp \
		      src/World.cpp \
		      src/simplex.cpp \
		      src/Image.cpp \
			  src/Terrain.cpp \
			  src/Inventory.cpp

LIB_MINECRAP_OBJS?=$(addprefix , $(LIB_MINECRAP_SOURCES:.cpp=.o))

libmc.a: $(LIB_MINECRAP_OBJS)
	ar rcs libmc.a $(LIB_MINECRAP_OBJS)

MINECRAP_SOURCES?=src/Main.cpp
MINECRAP_OBJS?=$(addprefix , $(MINECRAP_SOURCES:.cpp=.o))

mc: $(MINECRAP_OBJS) libmc.a
	@$(CC) $(CFLAGS) $(DEBUG) -o $@ $(MINECRAP_OBJS) libmc.a $(LDFLAGS) $(LIB_CVARS)

tests/test_chunk: src/test_chunk.o libmc.a
	@mkdir -p tests
	@$(CC) $(CFLAGS) $(DEBUG) -o $@ src/test_chunk.o libmc.a $(LDFLAGS) $(LIB_CVARS)


tests: tests/test_chunk

clean:
	rm -f src/*.o libmc.a mc
	rm -f tests/test_*
