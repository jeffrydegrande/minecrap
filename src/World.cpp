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

#define OUTSIDE_WORLD(x, y, z) \
    ((y < 0 || y > CHUNKY-1) || (x < 0 || x > (int)chunks->numRows() << 4) || (z < 0 || z > (int)chunks->numRows() << 4))

#define CHUNK_FROM_WORLD_COORDINATES(x, y) \
    chunks->get((x) / CHUNKX, (y) / CHUNKZ)

#define CHUNK_COORDINATES(x, y) \
    Vec2((x) % CHUNKX, (y) % CHUNKZ)


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

    Chunk *chunk = NULL;

	// pick a random spot in real world coordinates
	Vec2 size = this->getSize();
    Vec2 center((int)size.x / 2, (int)size.y / 2);

    // get the center chunk
    chunk = CHUNK_FROM_WORLD_COORDINATES(center.x, center.y);

    if (chunk->findSpawnLocation(playerSpawnLocation)) {
	    return new Player(this, playerSpawnLocation);
    }

    printf ("Can not find suitable player spawn location\n");
    exit(1);
}

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

GLubyte World::blockAt(int x, int y, int z) {
    if (OUTSIDE_WORLD(x, y, z))
        return AIR;

    Chunk *chunk = chunks->get(x / CHUNKX, z / CHUNKZ);
    return chunk->getBlock(Vec3((int)x % CHUNKX, y, (int)z % CHUNKZ));
}

GLubyte World::blockAt(const Vec3 &v) {
    return blockAt(v.x, v.y, v.z);
}

void World::changeBlock(const Vec3 &pos, GLubyte type)
{
    if (OUTSIDE_WORLD(pos.x, pos.y, pos.z))
        return;

    Chunk *chunk = CHUNK_FROM_WORLD_COORDINATES(pos.x, pos.z);
    Vec3 bp((int)pos.x % CHUNKX, pos.y, (int)pos.z % CHUNKZ);
    chunk->setBlock(bp.x, bp.y, bp.z, type);
    chunk->buildMesh();
}

void World::update() {
}

int World::render() {
	renderTerrain();
	// blocksRendered += sun->render();
	return 0;
}

bool World::outsideWorld(const Vec3 &pos)
{
    return (OUTSIDE_WORLD(pos.x, pos.y, pos.z));
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

void World::addBlock(Player *player)
{
	float maxDistance = 8.0;
    float distance = 0.0;

    Vec3 eye = player->getPosition();
	Vec3 ray, pos;
	GLubyte block = 0;

	// increase ray in increments of 0.1f until hitting a 
	// block or going beyond maximum distance
	while (distance < maxDistance) {
		distance += 0.1f;

		if (distance > maxDistance)
			return;

		ray = eye + (player->getDirection() * distance);
		pos = Vec3(floor(ray.x+0.5f), floor(ray.y+0.5f), floor(ray.z+0.5f));

		block = blockAt(pos);
		if (block != AIR) {
			break;
		}
	}

    pos.y += 1.0f;
	if (blockAt(pos) == AIR) {
        ConsoleLog("Adding Block: %0.2f, %0.2f, %0.2f", pos.x, pos.y, pos.z);
	    changeBlock(pos, ROCK);
	}
}

void World::removeBlock(Player *player)
{
	float maxDistance = 4.0;
    float distance = 0.0;

    Vec3 eye = player->getPosition();
	Vec3 ray;

	// increase ray in increments of 0.1f until hitting 
    // a block or going beyond maximum distance
	while (true) {
		ray = eye + (player->getDirection() * distance);

		if (distance > maxDistance)
			return;

		Vec3 pos(floor(ray.x+0.5f), floor(ray.y+0.5f), floor(ray.z+0.5f));
		GLubyte block = blockAt(pos);
		if (block != AIR && block != RED) {
            ConsoleLog("removing block: %0.2f, %0.2f, %0.2f", pos.x, pos.y, pos.z);
			changeBlock(pos, AIR);
			return;
		}
		distance += 0.1f;
	}
}