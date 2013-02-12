//
//  minecrap.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#ifndef Minecrap_minecrap_h
#define Minecrap_minecrap_h

#include <Windows.h>
#include <SDL.h>
#include <SDL_opengl.h>


#define FULLSCREEN false

#define DEGREES_TO_RADIANS        .017453292F
#define RADIANS_TO_DEGREES        57.29577951F
#define PI                        (3.1415926535f)


#define STRAFE_DISTANCE 1.0f


#define CHUNKX 16
#define CHUNKY 16
#define CHUNKZ 128

#define WATER_LEVEL 62

#define AIR   0
#define ROCK  1
#define GEMS  2
#define DIRT  3
#define GRASS 4
#define LAVA  5
#define WATER 6

#define RED   7

typedef struct Point3D_ {
    GLfloat x, y, z;
} Point3D;


#define foreach_xyz \
    for (int x=0; x<CHUNKX; x++) { \
        for (int y=0; y< CHUNKY; y++) { \
            for (int z=0; z < CHUNKZ; z++)

#define endforeach }}

#define foreach_xy \
	{ \
		for (int x=0; x < CHUNKX; x++) { \
			for(int y=0; y < CHUNKY; y++)

#define clamp(value, upper, lower)		max(min(value, (lower)), (upper))

#endif
