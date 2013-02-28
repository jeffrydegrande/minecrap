#ifndef MINECRAP_MESH_H
#define MINECRAP_MESH_H

struct vertex_t {
    float x, y, z, w;     //position
          ///nx, ny, nz, nw;  //normal
};


class Vec3;
class Mesh {
    ssize_t index;
    ssize_t vertexCount;

    unsigned int vbo;
    unsigned int vao;

    vertex_t* vertices;

    public:
        Mesh(ssize_t count); // number of vertices
        ~Mesh();

        void addCube(const Vec3 & pos);
        void finish();

        void build();
        void render();

};

#endif
