#ifndef MINECRAP_MODEL_H
#define MINECRAP_MODEL_H

#include <vector>
#include "Matrix.h"
#include "IndexedMesh.h"

class aiScene;
class aiMesh;
class aiNode;
class Texture;
class Model {

    int currentMeshIndex;

    public:

        Model(const char *filename);
        ~Model();

        void render();

    private:


        void initFromScene(const aiScene *scene);
        void extractMeshesFromNode(const aiScene *scene, const aiNode *node);
        void initMesh(unsigned int index, const aiMesh *mesh, const aiNode *node);
        void initMaterials(const aiScene *scene);

        std::vector<IndexedMesh> meshes;
        std::vector<Texture *> textures;
};

#endif
