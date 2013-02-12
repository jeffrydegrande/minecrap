//
//  Chunk.m
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

Chunk::Chunk(int x, int y, int seed) {
	this->seed = seed;
	this->worldX = x;
	this->worldY = y;
	this->generate();
}

Chunk::~Chunk() {
}

int Chunk::X() {
	return worldX;
}

int Chunk::Y() {
	return worldY;
}


int Chunk::groundLevel(int x, int y) {
	for (int z=CHUNKZ; z >= 0; z--) {
		GLubyte block = blocks[x][y][z];
		if (block != AIR) {
			return z;
		}
		if (z > 100)
			setBlock(x, y, z, RED);
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
	} endforeach

	generateTerrain();
	addDirt();
	addWaterLevel();
	addBedrock();
	// addMarkersAtBoundaries();
	summarize();
}

float terrainNoise(float x, float y, float frequency, float amplitude) {
	float result = 0;
	float amp = 1;
	float freq = 1;
	float max = 0;

	x*= 1/64.0f;
	y*= 1/64.0f;

	for (int i = 0; i < 8; i++) {
		result += noise2D(x * freq, y*freq) * amp;
		max += amp;
		freq *= frequency;
		amp *= amplitude;
	}

	result /= max;

	return result;
}

void Chunk::generateTerrain() {
	for(int x=0; x<CHUNKX; x++) {
		for(int y=0; y<CHUNKY; y++) {
			float maxHeight = terrainNoise(
				(float)(x + seed + (worldX << 4)),
				(float)(y + seed + (worldY << 4)),
				0.5f,
				0.5f
			) * 32 + WATER_LEVEL;

			assert(maxHeight <= CHUNKZ && maxHeight >= 0);
			
			blocks[x][y][(int)maxHeight] = ROCK;
			blocks[x][y][(int)maxHeight -1] = ROCK;

			/*
			for (int z = (int)maxHeight; z > 0; z--) {
				blocks[x][y][z] = ROCK;
			}
			*/
		}
	}
}

void Chunk::summarize() {
	int rock =0, dirt=0, air=0, test=0, empty=0;
	int blockCount = 0;

	foreach_xyz {
		switch(blocks[x][y][z]) {
		case AIR: air++; break;
		case RED: test++; break;
		case ROCK: rock++; break;
		case DIRT: dirt++; break;
		default: empty++;
		}
		blockCount++;
	} endforeach
}



void Chunk::addMarkersAtBoundaries() {
	blocks[0][0][0] = RED;  
	blocks[0][CHUNKY-1][0] = RED;
	blocks[CHUNKX-1][0][0] = RED;
	blocks[CHUNKX-1][CHUNKY-1][0] = RED;

	blocks[0][0][CHUNKZ-1] = RED;
	blocks[0][CHUNKY-1][CHUNKZ-1] = RED;
	blocks[CHUNKX-1][0][CHUNKZ-1] = RED;
	blocks[CHUNKX-1][CHUNKY-1][CHUNKZ-1] = RED;
}

void Chunk::addDirt() {
	foreach_xyz {
		int block = blocks[x][y][z];
		int ontop = blocks[x][y][z+1];
		if (block == ROCK && ontop == AIR) {
			blocks[x][y][z] = DIRT;
		}
	} endforeach
}

void Chunk::addBedrock() {
	foreach_xy {
		blocks[x][y][0] = ROCK;
	} endforeach;
}

void Chunk::addWaterLevel() {
	foreach_xy {
		blocks[x][y][WATER_LEVEL] = WATER;
	} endforeach;
}


bool Chunk::isBorderBlock(int x, int y, int z) {
	return (x == 0 || y == 0 || z == 0 || x == CHUNKX || y == CHUNKY || z == CHUNKZ);
}

bool Chunk::isExposedToAir(int x, int y, int z) {
	return (blocks[x+1][y][z] == AIR || blocks[x-1][y][z] == AIR
		|| blocks[x][y+1][z] == AIR || blocks[x][y-1][z] == AIR
		|| blocks[x][y][z+1] == AIR || blocks[x][y][z-1] == AIR);
}

int Chunk::renderBlock(int x, int y, int z) {
	if (blocks[x][y][z] == AIR)
		return 0;

	/*
	if (!isExposedToAir(x, y, z)) {
	return 0;
	}
	*/


	GLubyte block = blocks[x][y][z];

	if (block == AIR)
		return 0;

	glPushMatrix();
	glTranslatef((float)((this->worldX << 4) + x),
		(float)((this->worldY << 4) + y),
		(float)z);
	Block::render(block);
	glPopMatrix();
	return 1;
}