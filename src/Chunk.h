#ifndef MINECRAP_CHUNK_H
#define MINECRAP_CHUNK_H
#include "minecrap.h"
#include "Vec.h"
#include "Mesh.h"

#define CHUNKX 16
#define CHUNKY 128
#define CHUNKZ 16

#define B(x, y, z) blocks[x][y][z]

class Chunk
{
	GLubyte blocks[CHUNKX][CHUNKY][CHUNKZ];
	int worldX;
	int worldZ;
	int seed;

    Mesh *opaque;
    Mesh *transparent;

public:

	Chunk(int x, int y, int seed);
	~Chunk();

	void setBlock(int x, int y, int z, GLubyte type);

    GLubyte getBlock(const Vec3 &v) const;

    Vec3 inWorld(int x, int y, int z);

	int groundLevel(int x, int y);
    bool isGround(int x, int y, int z);

    int render();
	void generate();
	void summarize();

	int X();
	int Z();

    int vertexCount() const;

    void buildMesh();

private:

	int renderBlock(int x, int y, int z);

	void generateTerrain();
	void addDirt();
    void addSand();
	void addMarkersAtBoundaries();
	void addBedrock();
	void addWaterLevel();

	bool isExposedToAir(int x, int y, int z);
	bool isBorderBlock(int x, int y, int z);

};

inline int Chunk::vertexCount() const
{ return opaque->count() + transparent->count(); }

#endif