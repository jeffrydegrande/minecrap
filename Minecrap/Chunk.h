//
//  Chunk.h
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"

#define CHUNKX 16
#define CHUNKY 16
#define CHUNKZ 128

class Chunk
{
	GLubyte blocks[CHUNKX][CHUNKY][CHUNKZ];
	int worldX;
	int worldY;
	int seed;


public:

	Chunk(int x, int y, int seed);
	~Chunk();

	void setBlock(int x, int y, int z, GLubyte type);
	int renderBlock(int x, int y, int z);

	int groundLevel(int x, int y);


	void generate();
	void summarize();


	int X();
	int Y();


private:

	void generateTerrain();
	void addDirt();
	void addMarkersAtBoundaries();
	void addBedrock();
	void addWaterLevel();

	bool isExposedToAir(int x, int y, int z);
	bool isBorderBlock(int x, int y, int z);

};

// - (id) initWithWorldPosition:(int)x :(int)y :(int) worldSeed;
// - (int) renderBlock:(int)x :(int)y :(int)z;
// - (bool) isExposedToAir:(int)x :(int)y :(int)z;
// - (int) heightAtPosition:(int)x :(int)z;
// - (void) setBlock:(int)x :(int)y :(int)z :(GLubyte)blockType;