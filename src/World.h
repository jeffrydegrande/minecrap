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
#include "Vec.h"

class Sun;
class Vec2;
class Player;
class World
{
	int seed;
	Grid<Chunk *> *chunks;	
	Sun *sun;

public:
	World();
	World(int seed);
	~World();

	void init(int seed);
	Vec2 getSize();

	void update();
	int render();

	Player * spawnPlayer();

    bool isGround(int x, int y, int z);
    bool isGround(const Vec3 &v);

private:
	void generateChunks(int count);
	int renderTerrain();

	int renderChunk(Chunk *);
};