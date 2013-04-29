#ifndef MINECRAP_MESH_H
#define MINECRAP_MESH_H

#include "minecrap.h"

#include <cstddef>
#include <cstdlib>

struct vertex_t {
    float x, y, z,     // position
          nx, ny, nz,  // normal
          s, t, p;   // textures
};

struct vertex2_t {
    float x, y, z,     // position
          nx, ny, nz,  // normal
          s, t;   // textures
};

class Vec3;
class Mesh {
    int index;
    int vertexCount;

    GLuint vbo;
    GLuint vao;

    vertex_t* vertices;

    public:
        Mesh(int count); // number of vertices
        ~Mesh();

        void addCube(const Vec3 & pos, GLubyte kind, GLubyte faces);
        void finish();

        void build();
        void render(bool transparency);
        int count() const;

    private:
        void loadTextures();
        void addCubeFace(const Vec3 &pos, GLubyte kind, int start, int stop);
};

inline int Mesh::count() const
{ return index; }

#endif
