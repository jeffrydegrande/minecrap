//
//  Chunk.cpp
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"
#include "Color.h"
#include "Chunk.h"
#include "Vec.h"
#include "Mesh.h"

#include "simplex.h"
#include <assert.h>
#include <string>

Chunk::Chunk(int x, int z, int seed): mesh(NULL) {
    this->seed = seed;
    this->worldX = x;
    this->worldZ = z;
    this->generate();
}

Chunk::~Chunk() {
    if (mesh != NULL) {
        delete mesh;
        mesh = NULL;
    }
}

int Chunk::X() {
    return worldX;
}

int Chunk::Z() {
    return worldZ;
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

void Chunk::generate() {
    // printf( "Generating chunk \n" );
	memset(blocks, 0, sizeof(GLubyte)*CHUNKX*CHUNKY*CHUNKZ);

    generateTerrain();
    addDirt();
    addWaterLevel();
    addSand(); // need to do this after adding water
    addBedrock();
    // addMarkersAtBoundaries();
    buildMesh();
}

float terrainNoise(float x, float z, float frequency, float amplitude) {
    float result = 0;
    float amp = 1;
    float freq = 1;
    float max = 0;

    x*= 1/64.0f;
    z*= 1/64.0f;

    for (int i = 0; i < 8; i++) {
        result += noise2D(x * freq, z*freq) * amp;
        max += amp;
        freq *= frequency;
        amp *= amplitude;
    }

    result /= max;

    return result;
}

void Chunk::generateTerrain() {

    foreach_xz {
        float maxHeight = terrainNoise(
                (float)(x + seed + (worldX << 4)),
                (float)(z + seed + (worldZ << 4)),
                0.5f,
                0.5f
                ) * 32 + WATER_LEVEL;

        assert(maxHeight <= CHUNKY && maxHeight >= 0);

		B(x, (int)maxHeight, z) = ROCK;
		B(x, (int)maxHeight - 1, z) = ROCK;

    } endforeach;
}

void Chunk::addMarkersAtBoundaries() {
    B(0, 0, 0) = RED;
    B(0, 0, CHUNKZ-1) = RED;
    B(CHUNKX-1, 0, 0) = RED;
    B(CHUNKX-1, 0, CHUNKZ-1) = RED;

    B(0, CHUNKY-1, 0) = RED;
    B(0, CHUNKY-1, CHUNKZ-1) = RED;
    B(CHUNKX-1, CHUNKY-1, 0) = RED;
    B(CHUNKX-1, CHUNKY-1, CHUNKZ-1) = RED;
}

void Chunk::addDirt() {
	for (int y=0; y<CHUNKY; y++) {
		for (int z=0; z<CHUNKZ; z++) {
			for (int x=0; x<CHUNKX; x++) {
				if (B(x,y,z) == ROCK && B(x,y+1,z) == AIR) {
					B(x,y,z) = DIRT;
					B(x,y,z) = DIRT;
				}
			}
        }
    }
}

void Chunk::addBedrock() {
    foreach_xz {
        B(x,0,z) = ROCK;
    } endforeach;
}

void Chunk::addSand() {
    // everything on WATER_LEVEL that is not WATER or AIR
    foreach_xz {
        GLubyte block = B(x,WATER_LEVEL,z);
        if (block != WATER && block != AIR)
            B(x, WATER_LEVEL, z) = SAND;
    } endforeach;
}

void Chunk::addWaterLevel() {
    foreach_xz {
        int y = CHUNKY - 1;
        while (y > 0 && B(x,y,z) == AIR) {
            if (y <= WATER_LEVEL)
                B(x,y,z) = WATER;
            y--;
        }
    } endforeach;
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
    return Vec3((float)((this->worldX << 4) + x), (float)y, (float)((this->worldZ << 4) + z));
}

void Chunk::buildMesh() {
	int vertexCount = 0;

    // calculate the number of vertices need
	for (int y=0; y<CHUNKY; y++) {
		for (int z=0; z<CHUNKZ; z++) {
			for (int x=0; x<CHUNKX; x++) {
				if (B(x,y,z) == AIR) continue;
				vertexCount += 36; // 6 faces, 2 triangles/face, 3verts/trangle
			}
		}
	}

    mesh = new Mesh(vertexCount);
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
				if(z == CHUNKZ-1 || (z < CHUNKZ-1 && B(x  ,y  ,z+1) == AIR)) {
                    faces |= (1<<0);
                }

                // right face
                if(x == CHUNKX-1 || (x < CHUNKX-1 && B(x+1,y  ,z  ) == AIR)) {
                    faces |= (1<<1);
                }

                // back
                if(z == 0 || (z > 0 && B(x  ,y  ,z-1) == AIR)) {
                    faces |= (1<<2);
                }
				
                // left
                if(x == 0 || (x > 0 && B(x-1,y  ,z  ) == AIR)) {
                    faces |= (1<<3); // left
                }

				// top
                if(y == CHUNKY-1 || (y < CHUNKY-1 && B(x  ,y+1,z  ) == AIR)) {
                    faces |= (1<<4); // top
                }
				
                // bottom
                if(y == 0 || (y > 0 && B(x  ,y-1,z  ) == AIR)) {
                    faces |= (1<<5);
                }


                if (faces>0) {
					mesh->addCube(inWorld(x,y,z), block, faces);
                }
			}
		}
	}
    mesh->finish();
}

int Chunk::render() {
    if (mesh != NULL) {
        mesh->render();
    }
	return 1;
}

#undef B