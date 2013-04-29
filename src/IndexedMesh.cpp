#include "IndexedMesh.h"
#include "Matrix.h"
#include <cassert>

IndexedMesh::IndexedMesh()
{
    VAO = INVALID_OPENGL_VALUE;
    VBO = INVALID_OPENGL_VALUE;
    IBO = INVALID_OPENGL_VALUE;
    numIndices = 0;
    materialIndex = INVALID_MATERIAL;
}

IndexedMesh::~IndexedMesh()
{
    if (VAO != INVALID_OPENGL_VALUE)
        glDeleteBuffers(1, &VAO);

    if (VBO != INVALID_OPENGL_VALUE)
        glDeleteBuffers(1, &VBO);

    if (IBO != INVALID_OPENGL_VALUE)
        glDeleteBuffers(1, &IBO);

}

void IndexedMesh::init(const std::vector<struct vertex2_t>& vertices,
                       const std::vector<unsigned int>& indices)
{
    numIndices  = indices.size();

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex2_t) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // vertices
    glEnableVertexAttribArray(1); // normals
    glEnableVertexAttribArray(2); // textures

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex2_t), (GLvoid*)offsetof(struct vertex2_t, x));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex2_t), (GLvoid*)offsetof(struct vertex2_t, nx));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex2_t), (GLvoid*)offsetof(struct vertex2_t, s));

    glDisableVertexAttribArray(0); // vertices
    glDisableVertexAttribArray(1); // normals
    glDisableVertexAttribArray(2); // textures
}
