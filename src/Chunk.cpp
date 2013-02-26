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


const Vec3 cubeVerts[] =
{
  Vec3( 0.5, 0.5, 0.5), //0
  Vec3( 0.5, 0.5,-0.5), //1
  Vec3( 0.5,-0.5, 0.5), //2
  Vec3( 0.5,-0.5,-0.5), //3
  Vec3(-0.5, 0.5, 0.5), //4
  Vec3(-0.5, 0.5,-0.5), //5
  Vec3(-0.5,-0.5, 0.5), //6
  Vec3(-0.5,-0.5,-0.5)  //7
};

const Vec3 verts[] = //36 vertices total
{
  cubeVerts[0], cubeVerts[4], cubeVerts[6],  //front
  cubeVerts[0], cubeVerts[6], cubeVerts[2],  
  cubeVerts[1], cubeVerts[0], cubeVerts[2],  //right 
  cubeVerts[1], cubeVerts[2], cubeVerts[3],
  cubeVerts[5], cubeVerts[1], cubeVerts[3],  //back  
  cubeVerts[5], cubeVerts[3], cubeVerts[7],
  cubeVerts[4], cubeVerts[5], cubeVerts[7],  //left  
  cubeVerts[4], cubeVerts[7], cubeVerts[6],
  cubeVerts[4], cubeVerts[0], cubeVerts[1],  //top 
  cubeVerts[4], cubeVerts[1], cubeVerts[5],
  cubeVerts[6], cubeVerts[7], cubeVerts[3],  //bottom 
  cubeVerts[6], cubeVerts[3], cubeVerts[2],
}; 

const Vec3 right( 1.0f, 0.0f, 0.0f);
const Vec3 left(-1.0f, 0.0f, 0.0f);
const Vec3 top( 0.0f, 1.0f, 0.0f);
const Vec3 bottom( 0.0f,-1.0f, 0.0f);
const Vec3 front( 0.0f, 0.0f, 1.0f);
const Vec3 back( 0.0f, 0.0f,-1.0f);

const Vec3 normsArray[] = 
{
  front, front, front, front, front, front,
  right, right, right, right, right, right,
  back, back, back, back, back, back,
  left, left, left, left, left, left,
  top, top, top, top, top, top,
  bottom, bottom, bottom, bottom, bottom, bottom

};


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

/*
static GLint faces[12][3] =
{
    {0, 1, 2}, {0, 3, 2}, // front
    {2, 3, 7}, {2, 7, 6}, // top
    {5, 4, 6}, {5, 6, 7}, // back
    {4, 5, 1}, {4, 1, 0}, // bottom
    {4, 0, 2}, {4, 2, 6}, // left
    {1, 5, 7}, {1, 7, 3}  // right
};
*/

void Chunk::buildMesh() {
    int index = 0;
	vertexCount = 0;

    // calculate the number of vertices need
    foreach_xyz {
        if (B(x,y,z) == AIR)
            continue;
        vertexCount += 36; // 6 faces, 2 triangles/face, 3verts/trangle
    } endforeach;


    vertices = new struct vertex_t[vertexCount];

    foreach_xyz {
        if (B(x,y,z) == AIR)
            continue;

        Vec3 world = inWorld(x,y,z);

        for (int i=0; i<36; i++) {
            vertices[index].x = world.x + verts[i].x;
            vertices[index].y = world.y + verts[i].y;
            vertices[index].z = world.z + verts[i].z;

            vertices[index].nx = normsArray[i].x;
            vertices[index].ny = normsArray[i].y;
            vertices[index].nz = normsArray[i].z;
            // vertices[index].u = 0;
            // vertices[index].v = 0;
            index++;
        }
    } endforeach;

    printf( "%d vertices added, %d expected.\n", index, vertexCount );
    assert(index == vertexCount);

    printf( "allocating %ld kb\n", (vertexCount * sizeof(struct vertex_t)) / 1024 );

    glGenBuffers( 1, &vboVertex );

    glBindBuffer( GL_ARRAY_BUFFER, vboVertex);
    glBufferData( GL_ARRAY_BUFFER, vertexCount * sizeof(struct vertex_t), vertices, GL_STATIC_DRAW );
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TODO: texture buffers: see
    // http://nehe.gamedev.net/tutorial/vertex_buffer_objects/22002/

    delete [] vertices;
    vertices = NULL;
}

int Chunk::renderMesh() {
    return 0;
}

#define CHECK_GL_ERROR assert(GL_NO_ERROR == glGetError())

int Chunk::render() {

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dgreen);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, green);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertex);

    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(ATTRIB_VERTEX); // vertices

    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(ATTRIB_NORMAL); // normals

    glDrawArrays( GL_TRIANGLES, 0, vertexCount);

    glDisableVertexAttribArray(ATTRIB_VERTEX);
    glDisableVertexAttribArray(ATTRIB_NORMAL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
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