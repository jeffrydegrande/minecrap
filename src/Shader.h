#ifndef MINECRAP_SHADER_H
#define MINECRAP_SHADER_H

#include "minecrap.h"
#include <vector>

class Shader {
    GLuint program;
    std::vector<GLuint> shaders;

    public:
        void addVertexShader(const char *path);
        void addFragmentShader(const char *path);
        void done();

        void use();
        void dontUse();

    private:
        GLuint addShader(GLenum type, const char *path);
        void checkCompileStatus(GLuint shader);

};

#endif
