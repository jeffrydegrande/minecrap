#ifndef MINECRAP_MODEL_H
#define MINECRAP_MODEL_H

#include <vector>
#include "Matrix.h"

class aiScene;
class IndexedMesh;
class Model {
    struct mesh_t {
        unsigned int vao;
        unsigned int numFaces;
    };

    std::vector<struct mesh_t> meshes;

    unsigned int vao;
    unsigned int numFaces;

    Matrix4 worldMatrix;

    public:

        Model(const char *filename);
        ~Model();

        const Matrix4 &getWorldMatrix();

        void render();

    private:

        void buildMeshes(const aiScene *scene);

};

inline const Matrix4 &Model::getWorldMatrix() 
{ return worldMatrix; }

#endif
