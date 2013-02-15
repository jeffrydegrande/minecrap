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
#include "Console.h"
#include "Player.h"

#include "Vec2.h"

#define INITIAL_WORLD_SIZE 4

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

Vec2 World::getSize() {
	return Vec2(
		chunks->numRows() << 4,
		chunks->numColumns() << 4
		);
}


Player * World::spawnPlayer() {
	Vec3 playerSpawnLocation;

	// pick a random spot in real world coordinates
	Vec2 size = this->getSize();

	int x = rand() % (int)size.x;
	int y = rand() % (int)size.y;

	// find the block for this coordinate
	Chunk *chunk = chunks->get( x / CHUNKX, y / CHUNKY);

	// and translate the real world coordinates into chunk coordinates
	Vec2 chunkCoordinates = Vec2(x % CHUNKX, y % CHUNKY);

	int ground = chunk->groundLevel(chunkCoordinates.x, chunkCoordinates.y);

	chunk->setBlock(chunkCoordinates.x, chunkCoordinates.y, ground + 1, RED);
	
	playerSpawnLocation.x = (float)x;
	playerSpawnLocation.y = (float)y;
	playerSpawnLocation.z = (float)ground;

	return new Player(playerSpawnLocation);
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