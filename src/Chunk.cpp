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

#include "simplex.h"
#include <assert.h>

Chunk::Chunk(int x, int z, int seed) {
    this->seed = seed;
    this->worldX = x;
    this->worldZ = z;
    this->generate();
}

Chunk::~Chunk() {
}

int Chunk::X() {
    return worldX;
}

int Chunk::Z() {
    return worldZ;
}

bool Chunk::isGround(int x, int y, int z)
{
    GLubyte block = blocks[x][y][z];
    return (block != AIR);
}

int Chunk::groundLevel(int x, int z) {
    for (int y = CHUNKY-1; y > 0; y--) {
		GLubyte block = blocks[x][y][z];
		if (block != AIR) {
			return y;
		}
	}
	return 0;
}

void Chunk::setBlock(int x, int y, int z, GLubyte type)
{
    blocks[x][y][z] = type;
}

void Chunk::generate() {
    foreach_xyz {
        blocks[x][y][z] = 0;
    } endforeach;

	generateTerrain();
	addDirt();
	addWaterLevel();
	addBedrock();
	// addMarkersAtBoundaries();
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

        blocks[x][(int)maxHeight][z] = ROCK;
        blocks[x][(int)maxHeight -1][z] = ROCK;
    } endforeach;
}

void Chunk::summarize() {
	int rock =0, dirt=0, air=0, test=0, empty=0, water=0;
	int blockCount = 0;

	foreach_xyz {
		switch(blocks[x][y][z]) {
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
    setBlock(0, 0, 0, RED);
    setBlock(0, 0, CHUNKZ-1, RED);
    setBlock(CHUNKX-1, 0, 0, RED);
    setBlock(CHUNKX-1, 0, CHUNKZ-1, RED);

    setBlock(0, CHUNKY-1, 0, RED);
    setBlock(0, CHUNKY-1, CHUNKZ-1, RED);
    setBlock(CHUNKX-1, CHUNKY-1, 0, RED);
    setBlock(CHUNKX-1, CHUNKY-1, CHUNKZ-1, RED);
}

void Chunk::addDirt() {
    foreach_xyz {
        int block = blocks[x][y][z];
        int ontop = blocks[x][y+1][z];
        if (block == ROCK && ontop == AIR) {
            blocks[x][y][z] = DIRT;
        }
    } endforeach
}

void Chunk::addBedrock() {
    foreach_xz {
        blocks[x][0][z] = ROCK;
    } endforeach;
}

void Chunk::addWaterLevel() {
    foreach_xz {
        int y = CHUNKY - 1;
        while (y > 0 && blocks[x][y][z] == AIR) {
            if (y <= WATER_LEVEL)
                blocks[x][y][z] = WATER;
            y--;
        }
    } endforeach;
}

bool Chunk::isBorderBlock(int x, int y, int z) {
    return (x == 0 || y == 0 || z == 0 || x == CHUNKX || y == CHUNKY || z == CHUNKZ);
}

bool Chunk::isExposedToAir(int x, int y, int z) {
    return (blocks[x+1][y][z] == AIR || blocks[x-1][y][z] == AIR
        ||  blocks[x][y+1][z] == AIR || blocks[x][y-1][z] == AIR
        ||  blocks[x][y][z+1] == AIR || blocks[x][y][z-1] == AIR);
}

int Chunk::renderBlock(int x, int y, int z) {
    if (blocks[x][y][z] == AIR)
        return 0;
    /*
    if (blocks[x][y][z] == AIR || !isExposedToAir(x, y, z))
        return 0;
    */

    GLubyte block = blocks[x][y][z];

    if (block == AIR)
        return 0;

    glPushMatrix();
    glTranslatef((float)((this->worldX << 4) + x),
                 (float)y,
                 (float)((this->worldZ << 4) + z));
    Block::render(block);
    glPopMatrix();
    return 1;
}