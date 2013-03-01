#include "minecrap.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Vec.h"
#include "Color.h"
#include <cassert>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// data used to construct a 1x1 cube used by addCube
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

#include <iostream>
#include <cstdio>
#include <cstdlib>

Mesh::Mesh(int count): index(0), vertexCount(count) {
    vertices = new struct vertex_t[count];
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::addCube(const Vec3 & pos) {
    assert( index <= vertexCount );

    for (int i=0; i<36; i++) {
        // position
        vertices[index].x  = pos.x + verts[i].x;
        vertices[index].y  = pos.y + verts[i].y;
        vertices[index].z  = pos.z + verts[i].z;

        // color
        vertices[index].r  = 0.0f;
        vertices[index].g  = 1.0f;
        vertices[index].b  = 0.0f;
        vertices[index].a  = 1.0f;

        // normals
        vertices[index].nx = normsArray[i].x;
        vertices[index].ny = normsArray[i].y;
        vertices[index].nz = normsArray[i].z;
        index++;
    }
}

void Mesh::finish() {
    printf( "%d vertices added, %d expected.\n", index, vertexCount );
    assert(index == vertexCount);

    glGenVertexArrays(1, &vao );
    glBindVertexArray(vao);

    glGenBuffers( 1, &vbo );
    printf( "allocating %ld kb, %d cubes\n", 
            (vertexCount * sizeof(struct vertex_t)) / 1024, index / 36);

    // upload data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    printf("number of vertices:%d, sizeof vertices: %ld\n", vertexCount, sizeof(struct vertex_t));
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(struct vertex_t),
            vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // and get rid of the data on our side
    delete [] vertices;
    vertices = NULL;
}

void Mesh::render() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0); // vertices
    glEnableVertexAttribArray(1); // colors
    glEnableVertexAttribArray(2); // normals

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), 
            (GLvoid*)offsetof(struct vertex_t, x));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), 
            (GLvoid*)offsetof(struct vertex_t, r));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), 
            (GLvoid*)offsetof(struct vertex_t, nx));

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
