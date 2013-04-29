#include "minecrap.h"
#include "Model.h"
#include "Matrix.h"
#include "IndexedMesh.h"
#include "Texture.h"
#include "Image.h"

#include <string>
#include <map>

#include <cassert>

#include <assimp/assimp.hpp>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>


Model::Model(const char *filename)
    : currentMeshIndex(0)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename,
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace |
        aiProcess_FlipUVs |
        aiProcess_Triangulate);

    if (scene != NULL) {
        initFromScene(scene);
    }
}

Model::~Model()
{
}

void Model::initFromScene(const aiScene *scene)
{
    assert(scene != NULL);
    meshes.resize(scene->mNumMeshes);
    textures.resize(scene->mNumTextures);

    extractMeshesFromNode(scene, scene->mRootNode);
    initMaterials(scene);
}

void Model::extractMeshesFromNode(const aiScene *scene, const aiNode *node)
{
    printf( "Node %s: %d\n", node->mName.data, node->mNumMeshes);

    if (node->mNumMeshes != 0) {

        for (unsigned int i=0; i < node->mNumMeshes; i++) {

            const aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            initMesh(currentMeshIndex, mesh, node);
            currentMeshIndex++;
        }
    }
    if (node->mNumChildren > 0) {
        for (unsigned int i=0; i < node->mNumChildren; i++) {
            const aiNode *child = node->mChildren[i];
            extractMeshesFromNode(scene, child);
        }
    }
}

void Model::initMesh(unsigned int index, const aiMesh *mesh, const aiNode *node)
{
    printf( "Initializing mesh %d\n", index);
    meshes[index].materialIndex = mesh->mMaterialIndex;

    std::vector<vertex2_t> vertices;
    std::vector<unsigned int> indices;

    // set the transformation matrix;

    aiMatrix4x4 transform = node->mTransformation;
    transform.Transpose();

    Matrix4 m;
    for(int x=0; x<16; x++) {
        m[x] = *transform[x];
    }

    const aiVector3D Zero(0.0f, 0.0f, 0.0f);

    for (unsigned int i=0; i<mesh->mNumVertices; i++) {
        const aiVector3D* pos      = &(mesh->mVertices[i]);
        const aiVector3D* normal   = &(mesh->mNormals[i]);
        const aiVector3D* texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero;

        vertex2_t v;
        v.x  = pos->x;      v.y  = pos->y;      v.z = pos->z;
        v.nx = normal->x;   v.ny = normal->y;   v.nz = normal->z;
        v.s  = texCoord->x; v.t  = texCoord->y;
        vertices.push_back(v);
    }

    for (unsigned int i=0; i<mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        indices.push_back(face.mIndices[0]);
        indices.push_back(face.mIndices[1]);
        indices.push_back(face.mIndices[2]);
    }
    meshes[index].init(vertices, indices);
}

void Model::initMaterials(const aiScene *scene)
{
    if (textures.size() > 0) {
        for (unsigned int m=0; m < scene->mNumMaterials; m++) {
            int texIndex = 0;
            aiString path;  // filename
            aiMaterial *material = scene->mMaterials[m];
            aiReturn texFound = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);

            if (texFound == AI_SUCCESS) {
                int i = atoi(path.data + 1);
                printf ("material %d uses texture %d\n", m, i);

                textures[m] = NULL;
                aiTexture *texture = scene->mTextures[i];
                Image *image = new Image();
                image->load(texture->pcData, texture->mWidth);
                Texture *t = new Texture();
                t->load(GL_TEXTURE_2D, image);
                textures[m] = t;
            }
        }
    }
}

void Model::render()
{
    for (size_t i=0; i<meshes.size(); i++) {

        glBindVertexArray(meshes[i].VAO);
        glEnableVertexAttribArray(0); // vertices
        glEnableVertexAttribArray(1); // normals
        glEnableVertexAttribArray(2); // textures

        // activate texture
        const unsigned int materialIndex = meshes[i].materialIndex;

        if (materialIndex < textures.size() && textures[materialIndex]) {
            textures[materialIndex]->bind(GL_TEXTURE0);
		}
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, meshes[i].numIndices, GL_UNSIGNED_INT, 0);
        glEnable(GL_CULL_FACE);

        glDisableVertexAttribArray(0); // vertices
        glDisableVertexAttribArray(1); // normals
        glDisableVertexAttribArray(2); // textures
    }
}
