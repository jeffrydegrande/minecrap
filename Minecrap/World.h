//
//  World.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/2/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"
#include "Chunk.h"
#include "Grid.h"
#include "Vec3.h"

class Sun;
class World
{
	Vec3 playerSpawnLocation;
	int seed;
	Grid<Chunk *> *chunks;	
	Sun *sun;

public:
	World();
	World(int seed);
	~World();

	void init(int seed);
	size_t getSize();

	void update();
	int render();

private:
	void generateChunks(int count);
	void calculatePlayerSpawnLocation();
	int renderTerrain();

	int renderChunk(Chunk *);
};