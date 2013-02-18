//
//  Chunk.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"

#define CHUNKX 16
#define CHUNKY 128
#define CHUNKZ 16

class Chunk
{
	GLubyte blocks[CHUNKX][CHUNKY][CHUNKZ];
	int worldX;
	int worldZ;
	int seed;


public:

	Chunk(int x, int y, int seed);
	~Chunk();

	void setBlock(int x, int y, int z, GLubyte type);
	int renderBlock(int x, int y, int z);

	int groundLevel(int x, int y);
    bool isGround(int x, int y, int z);


	void generate();
	void summarize();


	int X();
	int Z();


private:

	void generateTerrain();
	void addDirt();
	void addMarkersAtBoundaries();
	void addBedrock();
	void addWaterLevel();

	bool isExposedToAir(int x, int y, int z);
	bool isBorderBlock(int x, int y, int z);
};