#include "Model.h"
#include "Matrix.h"

#include "minecrap.h"

#include "IndexedMesh.h"

#include <cassert>

#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

Model::Model(const char *filename)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
    if (scene != NULL)
        buildMeshes(scene);
}

Model::~Model()
{
}

void Model::render()
{
    for (size_t i=0; i<meshes.size(); i++) {
        glBindVertexArray(meshes[i].vao);
        glDrawElements(GL_TRIANGLES, meshes[i].numFaces * 3, GL_UNSIGNED_INT, 0);
    }
}

void Model::buildMeshes(const aiScene *scene)
{
    assert(scene != NULL);
    GLuint buffer;

    const struct aiNode *node = scene->mRootNode;
    aiMatrix4x4 matrix = node->mChildren[0]->mTransformation; // node->mTransformation;
    Matrix4 m;

    m[0]=matrix.a1; m[4]=matrix.a2; m[8] =matrix.a3; m[12]=matrix.a4;
    m[1]=matrix.b1; m[5]=matrix.b2; m[9] =matrix.b3; m[13]=matrix.b4;
    m[2]=matrix.c1; m[6]=matrix.c2; m[10]=matrix.c3; m[14]=matrix.c4;
    m[3]=matrix.d1; m[7]=matrix.d2; m[11]=matrix.d3; m[15]=matrix.d4;

    worldMatrix = m;
    worldMatrix.print();

    printf( "%d meshes\n",scene->mNumMeshes);

    // For each mesh
    for (unsigned int n = 0; n < scene->mNumMeshes; ++n) {
        const struct aiMesh* mesh = scene->mMeshes[n];
        // set the mesh

        // create array with faces
        // have to convert from Assimp format to array
        unsigned int *faceArray;
        faceArray = (unsigned int *)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
        unsigned int faceIndex = 0;

        for (unsigned int t = 0; t < mesh->mNumFaces; ++t)
        {
            const struct aiFace* face = &mesh->mFaces[t];

            memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(float));
            faceIndex += 3;
        }

        struct mesh_t im;
        im.numFaces = scene->mMeshes[n]->mNumFaces;

        // generate Vertex Array for mesh
        glGenVertexArrays(1,&im.vao);
        glBindVertexArray(im.vao);

        // buffer for faces
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->mNumFaces * 3, faceArray, GL_STATIC_DRAW);

        free((void*)faceArray);

        // buffer for vertex positions
        if(mesh->HasPositions())
        {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, 0, 0, 0);
        }

        // buffer for vertex normals
        if (mesh->HasNormals())
        {
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*mesh->mNumVertices, mesh->mNormals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, 0, 0, 0);
        }

        // unbind buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        meshes.push_back(im);
    }
}
