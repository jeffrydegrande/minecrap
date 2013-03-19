#include "IndexedMesh.h"
#include "Matrix.h"
#include <cassert>

IndexedMesh::IndexedMesh(unsigned int vao, int facesCount) :vao(vao),
                                                            facesCount(facesCount) {
}

IndexedMesh::~IndexedMesh()
{
}

void IndexedMesh::addFaceIndexes(unsigned int *faces, size_t count)
{
    (void) faces;
    (void) count;
}

void IndexedMesh::addVertices(float *vertices, size_t count)
{
    (void) vertices;
    (void) count;
}

void addNormals(float *normals, size_t count)
{
    (void) normals;
    (void) count;
}

#if 0
            // buffer for vertex texture coordinates
            float *texCoords = (float *)malloc(sizeof(float)*2*mesh->mNumVertices);
            for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {

                texCoords[k*2]   = mesh->mTextureCoords[0][k].x;
                texCoords[k*2+1] = mesh->mTextureCoords[0][k].y; 

            }
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*2*mesh->mNumVertices, texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(texCoordLoc);
            glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, 0, 0, 0);
#endif

void IndexedMesh::render()
{
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0); // vertices
    glEnableVertexAttribArray(1); // colors
    glDrawElements(GL_TRIANGLES, facesCount * 3, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0); // vertices
    glDisableVertexAttribArray(1); // colors

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glEnableVertexAttribArray(2); // normals
    // glEnableVertexAttribArray(3); // textures

/*
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, x));
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, r));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, nx));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex_t),
            (GLvoid*)offsetof(struct vertex_t, s));
*/
    assert(GL_NO_ERROR == glGetError());
}
