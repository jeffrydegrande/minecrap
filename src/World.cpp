//
//  World.cpp
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include <time.h>
#include <cstdlib>
#include <cassert>

#include "minecrap.h"
#include "World.h"
#include "Color.h"
#include "Sun.h"
#include "Console.h"
#include "Player.h"

#include "Vec.h"

#define INITIAL_WORLD_SIZE 16

World::World(): chunks(NULL) {
	init((unsigned int)time(NULL));
}

World::World(int seed): chunks(NULL) {
	init(seed);
}

World::~World() {
	delete sun;
	for (size_t x=0; x < chunks->numRows(); x++) {
		for (size_t y=0; y < chunks->numColumns(); y++) {
            Chunk *chunk = chunks->get(x, y);
            delete chunk;
        }
    }
    delete chunks;
}

void World::init(int seed) {
	srand(seed);
	this->seed = rand() % 65336;

	// Block::setup();
	sun = new Sun();
	this->generateChunks(INITIAL_WORLD_SIZE);
}

void World::generateChunks(int count) {
    int vertexCount = 0;
	chunks = new Grid<Chunk *>(count, count);
	for (int x=0; x<count; x++) {
		for (int y=0; y<count; y++) {
			Chunk *chunk = new Chunk(x, y, this->seed);
			chunks->set(x, y, chunk);
            vertexCount += chunk->vertexCount();
		}
	}
    printf("Total vertices: %d\n", vertexCount);
}

/*
 * return a 2d vector with the number of blocks in each direction
 */
Vec2 World::getSize() {
	return Vec2(
		(float)(chunks->numRows() << 4),
		(float)(chunks->numColumns() << 4)
		);
}

Player * World::spawnPlayer() {
	Vec3 playerSpawnLocation;

    int x=0, y=0;
    int ground=0;

    Chunk *chunk = NULL;

	// pick a random spot in real world coordinates
	Vec2 size = this->getSize();

    // If we get to the water level and we haven't encountered
    // terrain yet, we can't use this to spawn.
    while (ground <= WATER_LEVEL) {
        x = rand() % (int)size.x;
        y = rand() % (int)size.y;

        // find the block for this coordinate
        chunk = chunks->get( x / CHUNKX, y / CHUNKZ);

        // and translate the real world coordinates into chunk coordinates
        // y in 2d => z in 3d
        int chunkX = x % CHUNKX;
        int chunkY = y % CHUNKZ;

        // find the ground level
        ground = chunk->groundLevel(chunkX, chunkY);
    }

	playerSpawnLocation.x = (float)x;
	playerSpawnLocation.y = (float)ground;
	playerSpawnLocation.z = (float)y;

    /*
    chunk->setBlock(x, ground, y, RED);
    chunk->buildMesh();
    */

#ifdef SUPPORT_GLCONSOLE
    ConsoleLog("Spawn location: %0.2f, %0.2f, %0.2f\n",
            playerSpawnLocation.x, playerSpawnLocation.y, playerSpawnLocation.z);
#endif

	return new Player(this, playerSpawnLocation);
}


#define OUTSIDE_WORLD(x, y, z) \
    ((y < 0 || y > CHUNKY-1) || (x < 0 || x > (int)chunks->numRows() << 4) || (z < 0 || z > (int)chunks->numRows() << 4))

bool World::isGround(int x, int y, int z) {
    if (OUTSIDE_WORLD(x,y,z))
        return false;

    // get the chunk from the grid
    Chunk *chunk = chunks->get(x / CHUNKX, z / CHUNKZ);
    int chunkX = x % CHUNKX;
    int chunkZ = z % CHUNKZ;
    return chunk->isGround(chunkX, y, chunkZ);
}

bool World::isGround(const Vec3 &v) {
    return isGround(v.x, v.y, v.z);
}

GLubyte World::blockAt(const Vec3 &v) {
    if (OUTSIDE_WORLD(v.x, v.y, v.z))
        return AIR;

    Chunk *chunk = chunks->get(v.x / CHUNKX, v.z / CHUNKZ);
    return chunk->getBlock(Vec3((int)v.x % CHUNKX, v.y, (int)v.z % CHUNKZ));
}

void World::update() {
}

int World::render() {
	renderTerrain();
	// blocksRendered += sun->render();
	return 0;
}

int World::renderTerrain() {
	int renderedBlocksCount=0;

	for (size_t x=0; x < chunks->numRows(); x++) {
		for (size_t y=0; y < chunks->numColumns(); y++) {
			Chunk *chunk = chunks->get(x, y);
			renderedBlocksCount += chunk->render();
		}
	}
	return renderedBlocksCount;
}