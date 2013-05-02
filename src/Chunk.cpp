//
//  Chunk.cpp
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"
#include "Block.h"
#include "Color.h"
#include "Chunk.h"
#include "Vec.h"
#include "Mesh.h"
#include "Terrain.h"

#include "simplex.h"
#include <cmath>
#include <assert.h>
#include <string>

Chunk::Chunk(int x, int z, int seed): 
    opaque(NULL),
    transparent(NULL)
{
    this->seed = seed;
    this->worldX = x;
    this->worldZ = z;
    this->generate();
}

Chunk::~Chunk() {
    delete opaque;
    opaque = NULL;
    delete transparent;
    transparent = NULL;
}

bool Chunk::findSpawnLocation(Vec3 &location)
{
    for (int x=0; x < CHUNKX; x++) {
        for (int y=0; y < CHUNKZ; y++) {
            int ground = groundLevel(x, y);
            if (ground > 0) {
                location = inWorld(x, ground+1, y);
                return true;
            }
        }
    }
    return false;
}

bool Chunk::isGround(int x, int y, int z)
{
    assert(x >= 0 && x < CHUNKX);
    assert(y >= 0 && y < CHUNKY);
    assert(z >= 0 && z < CHUNKZ);

    return (B(x, y, z) != AIR);
}

int Chunk::groundLevel(int x, int z) {
    for (int y = CHUNKY-1; y > 0; y--) {
		if (B(x,y,z) != AIR) {
			return y;
		}
	}
	return 0;
}

void Chunk::setBlock(int x, int y, int z, GLubyte type)
{
    B(x,y,z) = type;
}

GLubyte Chunk::getBlock(const Vec3 &v) const
{
    return B((int)v.x, (int)v.y, (int)v.z);
}

GLubyte Chunk::getBlock(int x, int y, int z) const
{
    return B(x,y,z);
}

void Chunk::generate() {
	memset(blocks, 0, sizeof(GLubyte)*CHUNKX*CHUNKY*CHUNKZ);
    SimpleTerrain terrain;
    terrain.generate(*this);
    buildMesh();
}

bool Chunk::isBorderBlock(int x, int y, int z) {
    return (x == 0 || y == 0 || z == 0 || x == CHUNKX || y == CHUNKY || z == CHUNKZ);
}

bool Chunk::isExposedToAir(int x, int y, int z) {
    return (B(x+1,y,z) == AIR || B(x-1,y,z) == AIR
        ||  B(x,y+1,z) == AIR || B(x,y-1,z) == AIR
        ||  B(x,y,z+1) == AIR || B(x,y,z-1) == AIR);
}

inline Vec3 Chunk::inWorld(int x, int y, int z)
{
    return Vec3((float)((this->worldX << 4) + x),
               (float)y,
               (float)((this->worldZ << 4) + z));
}

void Chunk::countNumberOfVertices(int *transparent, int *opaque)
{
    // calculate the number of vertices need
	for (int y=0; y<CHUNKY; y++) {
		for (int z=0; z<CHUNKZ; z++) {
			for (int x=0; x<CHUNKX; x++) {
                GLuint block = B(x,y,z);
				if (block == AIR) continue;
                // 6 faces,2 triangles/face, 3verts/trangle
                if (block == WATER)
                    *transparent += 36;
                else
				    *opaque += 36;
			}
		}
	}
}

void Chunk::buildMesh() {
    int numberOfTransparentVertices=0;
    int numberOfOpaqueVertices=0;

    countNumberOfVertices(&numberOfTransparentVertices, &numberOfOpaqueVertices);
    opaque = new Mesh(numberOfOpaqueVertices);
    transparent = new Mesh(numberOfTransparentVertices);

    GLubyte block;
    GLubyte faces;

	for (int y=0; y<CHUNKY; y++) {
		for (int z=0; z<CHUNKZ; z++) {
			for (int x=0; x<CHUNKX; x++) {
				block = B(x,y,z);
				if (block == AIR)
					continue;

				assert(x >=0 && x<CHUNKX);
				assert(y >=0 && y<CHUNKY);
				assert(z >=0 && z<CHUNKZ);

				faces = 0;

                // front face
				if(z == CHUNKZ-1 || (z < CHUNKZ-1 && B(x  ,y  ,z+1) == AIR)) { // front
                    faces |= (1<<0);
                }

                // right face
                if(x == CHUNKX-1 || (x < CHUNKX-1 && B(x+1,y  ,z  ) == AIR)) { // right
                    faces |= (1<<1);
                }

                // back
                if(z == 0 || (z > 0 && B(x  ,y  ,z-1) == AIR)) { // back
                    faces |= (1<<2);
                }
				
                // left
                if(x == 0 || (x > 0 && B(x-1,y  ,z  ) == AIR)) { // left
                    faces |= (1<<3); // left
                }

				// top
                if(y == CHUNKY-1 || (y < CHUNKY-1 && B(x  ,y+1,z  ) == AIR)) { // top
                    faces |= (1<<4); // top
                }
				
                // bottom
                if(y == 0 || (y > 0 && B(x  ,y-1,z  ) == AIR)) { // bottom
                    faces |= (1<<5);
                }

                if (faces>0) {
                    if (block == WATER)
					    transparent->addCube(inWorld(x,y,z), block, faces);
                    else
                        opaque->addCube(inWorld(x,y,z), block, faces);
                }
			}
		}
	}

    opaque->finish();
    transparent->finish();
}

int Chunk::render() {
    if (opaque != NULL) {
        opaque->render(false);
    }
    if (transparent != NULL) {
        transparent->render(true);
    }
	return 1;
}

#undef B