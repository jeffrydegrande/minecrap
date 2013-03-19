#ifndef MINECRAP_MODEL_H
#define MINECRAP_MODEL_H

#include <vector>

class aiScene;
class IndexedMesh;
class Model {
    std::vector<IndexedMesh *> meshes;

    unsigned int vao;
    unsigned int numFaces;

    public:

        Model(const char *filename);
        ~Model();

        void render();

    private:

        void buildMeshes(const aiScene *scene);

};

#endif
