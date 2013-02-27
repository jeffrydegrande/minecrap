#include "minecrap.h"
#include "Graphics.h"
#include "Mesh.h"
#include "Vec3.h"
#include "Color.h"
#include <cassert>

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

Mesh::Mesh(ssize_t count): index(0), vertexCount(count) {
    vertices = new struct vertex_t[count];
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Mesh::addCube(const Vec3 & pos) {
    assert( index <= vertexCount );

    for (int i=0; i<36; i++) {
        vertices[index].x  = pos.x + verts[i].x;
        vertices[index].y  = pos.y + verts[i].y;
        vertices[index].z  = pos.z + verts[i].z;
        vertices[index].nx = normsArray[i].x;
        vertices[index].ny = normsArray[i].y;
        vertices[index].nz = normsArray[i].z;
        index++;
    }
}

void Mesh::finish() {
    printf( "%ld vertices added, %ld expected.\n", index, vertexCount );
    assert(index == vertexCount);

    glGenVertexArrays(1, &vao );
    glGenBuffers( 1, &vbo );

    printf( "allocating %ld kb, %ld cubes\n", (vertexCount * sizeof(struct vertex_t)) / 1024, index / 36);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(struct vertex_t), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // TODO: texture buffers: see
    // http://nehe.gamedev.net/tutorial/vertex_buffer_objects/22002/
    delete [] vertices;
    vertices = NULL;
}

void Mesh::render() {
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 90.0);
    glMaterialfv(GL_FRONT, GL_AMBIENT, dgreen);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, green);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(ATTRIB_VERTEX); // vertices
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t), BUFFER_OFFSET(12));
    glEnableVertexAttribArray(ATTRIB_NORMAL); // normals
    glDrawArrays( GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(ATTRIB_VERTEX);
    glDisableVertexAttribArray(ATTRIB_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
