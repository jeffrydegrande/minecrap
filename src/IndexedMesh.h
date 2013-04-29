#ifndef MINECRAP_INDEXED_MESH_H
#define MINECRAP_INDEXED_MESH_H

#include "Mesh.h"
#include "Matrix.h"

struct IndexedMesh
{
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;

    unsigned int materialIndex;
    unsigned int numIndices;

    IndexedMesh();
    ~IndexedMesh();

    void init(const std::vector<struct vertex2_t>& vertices,
              const std::vector<unsigned int>& indices);
};

#endif
