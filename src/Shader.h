#ifndef MINECRAP_SHADER_H
#define MINECRAP_SHADER_H

#include "minecrap.h"
#include <vector>
#include "Matrix.h"

class Shader {
    GLuint program;
    GLint perspectiveMatrixUniform;
    GLint cameraMatrixUniform;
    std::vector<GLuint> shaders;

    public:
        Shader();

        void addVertexShader(const char *path);
        void addFragmentShader(const char *path);
        void done();

        void setPerspectiveMatrix(Matrix4 &m);
        void setCameraMatrix(Matrix4 &m);

        void use();
        void dontUse();

    private:
        GLuint addShader(GLenum type, const char *path);
        void checkCompileStatus(GLuint shader);

};

#endif
