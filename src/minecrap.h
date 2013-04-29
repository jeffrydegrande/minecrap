//
//  minecrap.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#ifndef Minecrap_minecrap_h
#define Minecrap_minecrap_h


#ifdef _WIN32
#define NO_SDL_GLEXT
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES

#ifdef HAVE_APPLE_OPENGL_FRAMEWORK
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#else
#    ifdef _WIN32
#      include <windows.h>
#      include <cstddef>
#    endif

#    include <GL/gl.h>
#    include <GL/glu.h>
#
#    ifdef _win32
#      include <GL/wglext.h>
#    endif
#    include <GL/glext.h>
#endif

#ifdef _WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#endif

/* #ifdef __APPLE__ */
/* #include <GLUT/glut.h> */
/* #else */
/* #include <GL/GLUT.h> */
/* #endif */

#include <CVars/CVar.h>
#include "Console.h"

#include <cstdio>


#ifdef _WIN32
#define sleep(N)  Sleep(N)
#define snprintf   _snprintf_s
#else
#include <unistd.h>
#define sleep(N)   usleep(N * 1000)
#endif

#ifdef HAVE_APPLE_OPENGL_FRAMEWORK
#define glBindVertexArray glBindVertexArrayAPPLE
#define glGenVertexArrays glGenVertexArraysAPPLE
#define  glDeleteVertexArrays glDeleteVertexArraysAPPLE
#endif


#define DEGREES_TO_RADIANS        .017453292F
#define RADIANS_TO_DEGREES        57.29577951F
#define PI                        (3.1415926535f)


#define STRAFE_DISTANCE 1.0f

#define WATER_LEVEL 64

typedef struct Point3D_ {
    GLfloat x, y, z;
} Point3D;


#define foreach_xyz \
    for (int x=0; x<CHUNKX; x++) { \
        for (int y=0; y< CHUNKY; y++) { \
            for (int z=0; z < CHUNKZ; z++)

#define endforeach }}

#define foreach_xz \
	{ \
		for (int x=0; x < CHUNKX; x++) { \
			for(int z=0; z < CHUNKZ; z++)

#include <algorithm>
#define clamp(value, upper, lower)		(std::max)((std::min)(value, (lower)), (upper))


#define CHECK_OPENGL_ERRORS \
    { \
        GLenum error; \
        while (GL_NO_ERROR != (error=glGetError())) { \
            std::string s = reinterpret_cast<const char *>(gluErrorString(error)); \
            fprintf(stderr, "Error: %s\n", s.c_str()); \
            exit(1); \
        } \
    }

#endif
