#include "minecrap.h"
#include "Mesh.h"
#include "Vec.h"
#include "Color.h"
#include "Image.h"
#include "Block.h"
#include <cassert>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define TEXTURE_WIDTH 16
#define TEXTURE_HEIGHT 16
#define TEXTURE_NUM 1

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

const Vec3 right(1.0f, 0.0f, 0.0f);
const Vec3 left(-1.0f, 0.0f, 0.0f);
const Vec3 top(0.0f, 1.0f, 0.0f);
const Vec3 bottom(0.0f, -1.0f, 0.0f);
const Vec3 front(0.0f, 0.0f, 1.0f);
const Vec3 back(0.0f, 0.0f, -1.0f);

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

void Mesh::addCubeFace(const Vec3 &pos, GLubyte kind, int start, int stop)
{
    assert( index <= vertexCount );

    for (int i=start; i<stop; i++) {
        // position
        vertices[index].x  = pos.x + verts[i].x;
        vertices[index].y  = pos.y + verts[i].y;
        vertices[index].z  = pos.z + verts[i].z;

        // vertices[index].a  = 1.0f;
        switch (i % 6) {
            case 0:
            case 3:
                vertices[index].s = 0.0f;
                vertices[index].t = 0.0f;
                break;

            case 1:
                vertices[index].s = 1.0f;
                vertices[index].t = 0.0f;
                break;

            case 2:
            case 4:
                vertices[index].s = 1.0f;
                vertices[index].t = 1.0f;
                break;

            case 5:
                vertices[index].s = 0.0f;
                vertices[index].t = 1.0f;
                break;
        }

        vertices[index].p = 0.0f;

        // color
        switch (kind) {
        case ROCK:
            vertices[index].p  = 4.0f;
            break;
        case GRASS:
        case DIRT:
            vertices[index].p = (start == 24) ? 1.0f : 0.0f;
            break;
        case WATER:
            vertices[index].a  = 0.4f;
            vertices[index].p = 2.0f;
            break;
        case SAND:
            vertices[index].p = 3.0f;
            break;
        case BEDROCK:
            vertices[index].p = 5.0f;
            break;

        case RED:
            vertices[index].p = 6.0f;
            break;
        case LAVA:
            vertices[index].p = 7.0f;
            break;
        }

        // normals
        vertices[index].nx = normsArray[i].x;
        vertices[index].ny = normsArray[i].y;
        vertices[index].nz = normsArray[i].z;
        index++;
    }
}

void Mesh::addCube(const Vec3 & pos, GLubyte kind, GLubyte faces) {
    if (faces & (1<<0))
        addCubeFace(pos, kind, 0, 6);
    if (faces & (1<<1))
        addCubeFace(pos, kind, 6, 12);
    if (faces & (1<<2))
        addCubeFace(pos, kind, 12, 18);
    if (faces & (1<<3))
        addCubeFace(pos, kind, 18, 24);
    if (faces & (1<<4))
        addCubeFace(pos, kind, 24, 30);
    if (faces & (1<<5))
        addCubeFace(pos, kind, 30, 36);
}

void Mesh::finish() {
    assert(index <= vertexCount);

    glGenVertexArrays(1, &vao );
    CHECK_OPENGL_ERRORS(__LINE__);

    glBindVertexArray(vao);
    CHECK_OPENGL_ERRORS(__LINE__);

    glGenBuffers( 1, &vbo );
    CHECK_OPENGL_ERRORS(__LINE__);
    /* printf("Allocating %ld kb, ?? cubes, %d vertices, expected %d\n", */
    /*     (index * sizeof(struct vertex_t)) / 1024, index, vertexCount); */

    // upload data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    CHECK_OPENGL_ERRORS(__LINE__);
    glBufferData(GL_ARRAY_BUFFER, index * sizeof(struct vertex_t), vertices, GL_STATIC_DRAW);
    CHECK_OPENGL_ERRORS(__LINE__);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    CHECK_OPENGL_ERRORS(__LINE__);

    // and get rid of the data on our side
    delete [] vertices;
    vertices = NULL;
}

void Mesh::render(bool transparency) {
    glBindVertexArray(vao);

    if (transparency) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0); // vertices
    glEnableVertexAttribArray(1); // colors
    glEnableVertexAttribArray(2); // normals
    glEnableVertexAttribArray(3); // textures

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, x));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, r));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, nx));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, s));

    glDrawArrays(GL_TRIANGLES, 0, index);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    if (transparency) {
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
    }
    CHECK_OPENGL_ERRORS(__LINE__);
}
