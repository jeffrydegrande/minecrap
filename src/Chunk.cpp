//
//  Chunk.cpp
//  Minecrap
//
//  Created by Jeffry Degrande on 2/4/13.
//  Copyright (c) 2013 Jeffry Degrande. All rights reserved.
//

#include "minecrap.h"
#include "Block.h"
#include "Color.h"
#include "Chunk.h"
#include "Vec3.h"
#include "Graphics.h"

#include "simplex.h"
#include <assert.h>
#include <string>

#define B(x,y,z)  blocks[x][y][z]

Chunk::Chunk(int x, int z, int seed) :vertexCount(0) {
    this->seed = seed;
    this->worldX = x;
    this->worldZ = z;
    this->generate();
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &vboVertex);
}

int Chunk::X() {
    return worldX;
}

int Chunk::Z() {
    return worldZ;
}

bool Chunk::isGround(int x, int y, int z)
{
    assert(x >= 0 && x < CHUNKX);
    assert(y >= 0 && y < CHUNKY);
    assert(z >= 0 && z < CHUNKZ);

    return (B(x, y, z) != AIR);
}

int Chunk::groundLevel(int x, int z) {
    for (int y = CHUNKY-1; y > 0; y--) {
		if (B(x,y,z) != AIR) {
			return y;
		}
	}
	return 0;
}

void Chunk::setBlock(int x, int y, int z, GLubyte type)
{
    B(x,y,z) = type;
}

void Chunk::generate() {
    printf( "Generating chunk \n" );
    foreach_xyz {
        B(x,y,z) = 0;
    } endforeach;

    generateTerrain();
    addDirt();
    addWaterLevel();
    addBedrock();
    // addMarkersAtBoundaries();
    buildMesh();
}

float terrainNoise(float x, float z, float frequency, float amplitude) {
    float result = 0;
    float amp = 1;
    float freq = 1;
    float max = 0;

    x*= 1/64.0f;
    z*= 1/64.0f;

    for (int i = 0; i < 8; i++) {
        result += noise2D(x * freq, z*freq) * amp;
        max += amp;
        freq *= frequency;
        amp *= amplitude;
    }

    result /= max;

    return result;
}

void Chunk::generateTerrain() {

    foreach_xz {
        float maxHeight = terrainNoise(
                (float)(x + seed + (worldX << 4)),
                (float)(z + seed + (worldZ << 4)),
                0.5f,
                0.5f
                ) * 32 + WATER_LEVEL;

        assert(maxHeight <= CHUNKY && maxHeight >= 0);

        B(x, (int)maxHeight, z) = ROCK;
        B(x, (int)maxHeight - 1, z) = ROCK;
    } endforeach;
}

void Chunk::summarize() {
	int rock =0, dirt=0, air=0, test=0, empty=0, water=0;
	int blockCount = 0;

	foreach_xyz {
		switch(B(x,y,z)) {
            case AIR: air++; break;
            case RED: test++; break;
            case ROCK: rock++; break;
            case DIRT: dirt++; break;
            case WATER: water++; break;
            default: 
                empty++;
		}
		blockCount++;
	} endforeach

	ConsoleLog("Chunk %d, %d: %d blocks (%d rock, %d dirt, %d air, %d water, %d test, %d empty)", 
               this->worldX, this->worldZ, blockCount, rock, dirt, air, water, test, empty);
}


void Chunk::addMarkersAtBoundaries() {
    B(0, 0, 0) = RED;
    B(0, 0, CHUNKZ-1) = RED;
    B(CHUNKX-1, 0, 0) = RED;
    B(CHUNKX-1, 0, CHUNKZ-1) = RED;

    B(0, CHUNKY-1, 0) = RED;
    B(0, CHUNKY-1, CHUNKZ-1) = RED;
    B(CHUNKX-1, CHUNKY-1, 0) = RED;
    B(CHUNKX-1, CHUNKY-1, CHUNKZ-1) = RED;
}

void Chunk::addDirt() {
    foreach_xyz {
        if (B(x,y,z) == ROCK && B(x,y+1,z) == AIR) {
            B(x,y,z) = DIRT;
            B(x,y,z) = DIRT;
        }
    } endforeach
}

void Chunk::addBedrock() {
    foreach_xz {
        B(x,0,z) = ROCK;
    } endforeach;
}

void Chunk::addWaterLevel() {
    foreach_xz {
        int y = CHUNKY - 1;
        while (y > 0 && B(x,y,z) == AIR) {
            if (y <= WATER_LEVEL)
                B(x,y,z) = WATER;
            y--;
        }
    } endforeach;
}

bool Chunk::isBorderBlock(int x, int y, int z) {
    return (x == 0 || y == 0 || z == 0 || x == CHUNKX || y == CHUNKY || z == CHUNKZ);
}

bool Chunk::isExposedToAir(int x, int y, int z) {
    return (B(x+1,y,z) == AIR || B(x-1,y,z) == AIR
        ||  B(x,y+1,z) == AIR || B(x,y-1,z) == AIR
        ||  B(x,y,z+1) == AIR || B(x,y,z-1) == AIR);
}

inline Vec3 Chunk::inWorld(int x, int y, int z)
{
    return Vec3((this->worldX << 4) + x, y, (this->worldZ << 4) + z);
}

static GLint faces[12][3] =
{
    {0, 1, 2}, {0, 3, 2}, // front
    {2, 3, 7}, {2, 7, 6}, // top
    {5, 4, 6}, {5, 6, 7}, // back
    {4, 5, 1}, {4, 1, 0}, // bottom
    {4, 0, 2}, {4, 2, 6}, // left
    {1, 5, 7}, {1, 7, 3}  // right
};

static GLfloat n[12][3] =
{
	{-1.0f,  0.0f,  0.0f}, // front
	{-1.0f,  0.0f,  0.0f}, // front
	{ 0.0f,  1.0f,  0.0f}, // top
	{ 0.0f,  1.0f,  0.0f}, // top
	{ 1.0f,  0.0f,  0.0f}, // back
	{ 1.0f,  0.0f,  0.0f}, // back
	{ 0.0f, -1.0f,  0.0f}, // bottom
	{ 0.0f, -1.0f,  0.0f}, // bottom
	{ 0.0f,  0.0f,  1.0f}, // left
	{ 0.0f,  0.0f,  1.0f}, // left
	{ 0.0f,  0.0f, -1.0f}, // right
	{ 0.0f,  0.0f, -1.0f}  // right
};

void Chunk::buildMesh() {
    int index = 0;
	vertexCount = 0;

    // calculate the number of vertices need
    foreach_xyz {
        if (B(x,y,z) == AIR)
            continue;
        vertexCount += 36; // 6 faces, 2 triangles/face, 3verts/trangle
    } endforeach;

    GLfloat v[8][3];

    //

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -0.5f;  // x - front
    v[4][0] = v[5][0] = v[6][0] = v[7][0] =  0.5f;  // x - back

    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -0.5f;  // y - top
    v[2][1] = v[3][1] = v[6][1] = v[7][1] =  0.5f;  // y - bottom

    /*
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -0.5f;  // z - left
    v[1][2] = v[2][2] = v[5][2] = v[6][2] =  0.5f;  // z - right
    */

    v[4][2] = v[0][2] = v[2][2] = v[6][2] = -0.5f;  // z - left
    v[1][2] = v[5][2] = v[7][2] = v[3][2] =  0.5f;  // z - right

    vertices = new struct vertex_t[vertexCount];
    foreach_xyz {
        if (B(x,y,z) == AIR)
            continue;

        Vec3 world = inWorld(x,y,z);

        // for every triangle making up the 6 planes
        for (int i=11; i>=0; i--) {
            // for every vertex in that triangle
            for (int j=0; j < 3; j++) {
                vertices[index].x = world.x + v[faces[i][j]][0];
                vertices[index].y = world.y + v[faces[i][j]][1];
                vertices[index].z = world.z + v[faces[i][j]][2];

                vertices[index].nx = n[i][0];
                vertices[index].ny = n[i][1];
                vertices[index].nz = n[i][2];
                vertices[index].u = 0;
                vertices[index].v = 0;
                index++;
            }
        }
    } endforeach;
    printf( "%d vertices added, %d expected.\n", index, vertexCount );
    assert(index == vertexCount);

    printf( "allocating %ld kb\n", (vertexCount * sizeof(struct vertex_t)) / 1024 );

    glGenBuffers( 1, &vboVertex );
    glBindBuffer( GL_ARRAY_BUFFER, vboVertex);
    glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof(struct vertex_t),
					 vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TODO: texture buffers: see
    // http://nehe.gamedev.net/tutorial/vertex_buffer_objects/22002/

    // delete [] vertices;
    // vertices = NULL;
}

int Chunk::renderMesh() {
    return 0;
}

#define CHECK_GL_ERROR assert(GL_NO_ERROR == glGetError())

int Chunk::render() {
    for (int i=0; i < vertexCount; i+=3) {
        glPushMatrix();
        glBegin(GL_TRIANGLES);
            glMaterialfv(GL_FRONT, GL_SPECULAR, white);
            glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
            glMaterialfv(GL_FRONT, GL_AMBIENT, dblue);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);

            glNormal3f(vertices[i].nx, vertices[i].ny, vertices[i].nz);
            glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

            glNormal3f(vertices[i+1].nx, vertices[i+1].ny, vertices[i+1].nz);
            glVertex3f(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);

            glNormal3f(vertices[i+2].nx, vertices[i+2].ny, vertices[i+2].nz);
            glVertex3f(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
        glEnd();
        glPopMatrix();
    }

    CHECK_GL_ERROR;

    /*
	glBindBuffer(GL_ARRAY_BUFFER, vboVertex);

	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer( GL_FLOAT, sizeof(float)*8, (float*)(sizeof(float)*3));
	glVertexPointer( 3, GL_FLOAT, sizeof(float)*8, NULL );

	glDrawArrays( GL_TRIANGLES, 0, vertexCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    */

	/*
	int renderedBlocksCount = 0;
	foreach_xyz {
		renderedBlocksCount += renderBlock(x, y, z);
	} endforeach

	return renderedBlocksCount;
	*/
	return 1;
}

int Chunk::renderBlock(int x, int y, int z) {

    int ret = 0;
    GLubyte block = B(x,y,z);

    if (block == AIR)
        return 0;

    Vec3 world = inWorld(x, y, z);
    glPushMatrix();
    glTranslatef(world.x, world.y, world.z);
    if (graphics->withinFrustum(world.x, world.y, world.z, 1.0f)) {
        Block::render(block);
        ret = 1;
    }
    glPopMatrix();
    return ret;
}

#undef B