#ifndef MINECRAP_MESH_H
#define MINECRAP_MESH_H

#include "minecrap.h"

#include <cstddef>
#include <cstdlib>

struct vertex_t {
    float x, y, z, w;     //position
          ///nx, ny, nz, nw;  //normal
};


class Vec3;
class Mesh {
    int index;
    int vertexCount;

    unsigned int vbo;
    unsigned int vao;

    vertex_t* vertices;

    public:
        Mesh(int count); // number of vertices
        ~Mesh();

        void addCube(const Vec3 & pos);
        void finish();

        void build();
        void render();

};

#endif