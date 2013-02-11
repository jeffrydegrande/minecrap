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

class Sun;
class World
{
	Point3D playerSpawnLocation;
	int seed;
	Grid<Chunk *> *chunks;	
	Sun *sun;

public:
	World();
	World(int seed);
	~World();

	void init(int seed);
	unsigned int getSize();

	int render();

private:
	void generateChunks(int count);
	void calculatePlayerSpawnLocation();
	int renderTerrain();

	int renderChunk(Chunk *);
};