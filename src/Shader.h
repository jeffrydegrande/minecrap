#ifndef MINECRAP_SHADER_H
#define MINECRAP_SHADER_H

#include "minecrap.h"
#include <vector>
#include "Matrix.h"
#include "Vec.h"

#include <map>
#include <string>

class Shader {
    GLuint program;

    std::vector<GLuint> shaders;
    std::map<std::string, GLint> uniforms;

    public:
        Shader();
        ~Shader();

        void addVertexShader(const char *path);
        void addFragmentShader(const char *path);
        void done();

        void setUniform(const char *name, Matrix4 &m);
        void setUniform(const char *name, const Matrix4 &m);
        void setUniform(const char *name, Matrix3 &m);
        void setUniform(const char *name, Vec3 &v);
        void setUniform(const char *name, Vec4 &v);
        void setUniform(const char *name, int i);

        void setDirectionToLight(Vec4 &v);

        void use();
        void dontUse();

    private:
        GLint getUniformLocation(const char *name);
        GLuint addShader(GLenum type, const char *path);
        void checkCompileStatus(GLuint shader);

};

class UseShader {
    Shader & shader;

    public:
        UseShader(Shader &shader): shader(shader) {
            shader.use();
        }

        ~UseShader() {
            shader.dontUse();
        }

    private:
        UseShader(const Shader &shader);
        UseShader &operator=(const Shader &shader);
};

#endif
