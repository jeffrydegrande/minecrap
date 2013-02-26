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
#include "Vec3.h"
#include "Graphics.h"
#include "Mesh.h"

#include "simplex.h"
#include <assert.h>
#include <string>

#define B(x,y,z)  blocks[x][y][z]

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
    printf( "Generating chunk \n" );
    foreach_xyz {
        B(x,y,z) = 0;
    } endforeach;

    generateTerrain();
    addDirt();
    addWaterLevel();
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

void Chunk::summarize() {
	int rock =0, dirt=0, air=0, test=0, empty=0, water=0;
	int blockCount = 0;

	foreach_xyz {
		switch(B(x,y,z)) {
            case AIR: air++; break;
            case RED: test++; break;
            case ROCK: rock++; break;
            case DIRT: dirt++; break;
            case WATER: water++; break;
            default: 
                empty++;
		}
		blockCount++;
	} endforeach

	ConsoleLog("Chunk %d, %d: %d blocks (%d rock, %d dirt, %d air, %d water, %d test, %d empty)", 
               this->worldX, this->worldZ, blockCount, rock, dirt, air, water, test, empty);
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
    foreach_xyz {
        if (B(x,y,z) == ROCK && B(x,y+1,z) == AIR) {
            B(x,y,z) = DIRT;
            B(x,y,z) = DIRT;
        }
    } endforeach
}

void Chunk::addBedrock() {
    foreach_xz {
        B(x,0,z) = ROCK;
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
    return Vec3((this->worldX << 4) + x, y, (this->worldZ << 4) + z);
}

void Chunk::buildMesh() {
	ssize_t vertexCount = 0;

    // calculate the number of vertices need
    foreach_xyz {
        if (B(x,y,z) == AIR) continue;
        vertexCount += 36; // 6 faces, 2 triangles/face, 3verts/trangle
    } endforeach;

    mesh = new Mesh(vertexCount);
    foreach_xyz {
        if (B(x,y,z) == AIR)
            continue;
        mesh->addCube(inWorld(x,y,z));
    } endforeach;
    mesh->finish();
}

#define CHECK_GL_ERROR assert(GL_NO_ERROR == glGetError())

int Chunk::render() {
    if (mesh != NULL)
        mesh->render();
	return 1;
}

int Chunk::renderBlock(int x, int y, int z) {

    int ret = 0;
    GLubyte block = B(x,y,z);

    if (block == AIR)
        return 0;

    Vec3 world = inWorld(x, y, z);
    glPushMatrix();
    glTranslatef(world.x, world.y, world.z);
    if (graphics->withinFrustum(world.x, world.y, world.z, 1.0f)) {
        Block::render(block);
        ret = 1;
    }
    glPopMatrix();
    return ret;
}

#undef B