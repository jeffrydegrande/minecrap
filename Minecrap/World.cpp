//
//  World.cpp
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include <time.h>
#include <stdlib.h>

#include "minecrap.h"
#include "World.h"
#include "Color.h"
#include "Sun.h"
#include "Block.h"

#define INITIAL_WORLD_SIZE 1

World::World(): chunks(NULL) {
	init((unsigned int)time(NULL));
}

World::World(int seed): chunks(NULL) {
	init(seed);
}

World::~World() {
	delete sun;
	sun = NULL;
}

void World::init(int seed) {
	srand(seed);
	this->seed = rand() % 65336;

	Block::setup();
	sun = new Sun();

	this->generateChunks(INITIAL_WORLD_SIZE);
	this->calculatePlayerSpawnLocation();
}

void World::generateChunks(int count) {

	chunks = new Grid<Chunk *>(count, count);

	for (int x=0; x<count; x++) {
		for (int y=0; y<count; y++) {
			Chunk *chunk = new Chunk(x, y, this->seed);
			chunks->set(x, y, chunk);
		}
	}
}

size_t World::getSize() {
	return chunks->actualSize();
}

#include <sstream>

void World::calculatePlayerSpawnLocation() {
	size_t size = this->getSize();
	
	int x = rand() % size;
	int y = rand() % size;

	Chunk *chunk = chunks->get( x / CHUNKX, y / CHUNKY);

	int xInChunk = x % CHUNKX;
	int yInChunk = y % CHUNKY;
	
	int ground = chunk->groundLevel(xInChunk, yInChunk);
	
	chunk->setBlock(xInChunk, yInChunk, 0, WATER);
	chunk->setBlock(xInChunk, yInChunk, ground, RED);

	std::ostringstream s;
	s << x << ", " << y << ", " << ground;
	SDL_WM_SetCaption(s.str().c_str(), NULL);

	playerSpawnLocation.x = (float)x;
	playerSpawnLocation.y = (float)y;
	playerSpawnLocation.z = (float)ground;


#if 0
	NSUInteger size = [self size];

	// we need an x and a z that are randoms within the world size

	int x = rand() % size;
	int z = rand() % size;

	// find the chunk where x,z can be found
	int xrow = x / 16;
	int zrow = z / 16;

	Chunk *chunk = [self findChunkAtPosition:xrow :zrow];

	int xChunk = x % 16;
	int zChunk = z % 16;

	int y = [chunk heightAtPosition:xChunk :zChunk];

	[chunk setBlock:xChunk :(y+1) :zChunk :RED];

	playerSpawnPoint.x = x;
	playerSpawnPoint.y = y+1;
	playerSpawnPoint.z = z;

	NSLog(@"spawning at %d,%d,%d (found in chunk %d, %d / %d, %d)", x, y, z, xrow, zrow, xChunk, zChunk);
#endif
}

void World::update() {
}

int World::render() {
	int blocksRendered = 0;
	blocksRendered += renderTerrain();
	blocksRendered += sun->render();
	return blocksRendered;
}

int World::renderTerrain() {
	int renderedBlocksCount=0;

	for (size_t x=0; x < chunks->numRows(); x++) {
		for (size_t y=0; y < chunks->numColumns(); y++) {
			Chunk *chunk = chunks->get(x, y);
			renderedBlocksCount += this->renderChunk(chunk);
		}
	}
    
	return renderedBlocksCount;
}

int World::renderChunk(Chunk *chunk) {
	int renderedBlocksCount = 0;
	foreach_xyz {
		renderedBlocksCount += chunk->renderBlock(x, y, z);
	} endforeach

	return renderedBlocksCount;
}