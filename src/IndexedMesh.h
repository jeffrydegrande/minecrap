#ifndef MINECRAP_INDEXED_MESH_H
#define MINECRAP_INDEXED_MESH_H

#include "Mesh.h"
#include "Matrix.h"

class IndexedMesh
{
    GLuint vao;

    vertex_t* vertices;
    int facesCount;

    Matrix4 worldMatrix;

    friend class Model;

    protected:
        void addFaceIndexes(unsigned int *faces, size_t count);
        void addVertices(float *vertices, size_t count);
        void addNormals(float *normals, size_t count);

    public:
        IndexedMesh(unsigned int, int facesCount);
        virtual ~IndexedMesh();
        const Matrix4 &getWorldMatrix();

        void render();
};

inline const Matrix4 &IndexedMesh::getWorldMatrix() 
{ return worldMatrix; }

#endif
