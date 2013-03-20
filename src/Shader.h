#ifndef MINECRAP_SHADER_H
#define MINECRAP_SHADER_H

#include "minecrap.h"
#include <vector>
#include "Matrix.h"
#include "Vec.h"

class Shader {
    GLuint program;

    std::vector<GLuint> shaders;

    public:
        Shader();
        ~Shader();

        void addVertexShader(const char *path);
        void addFragmentShader(const char *path);
        void done();

        GLint getUniformLocation(const char *name) const;

        void setUniformMatrix3(GLint uniformLocation, Matrix3 &m);
        void setUniformMatrix4(GLint uniformLocation, Matrix4 &m);
        void setUniformMatrix4(GLint uniformLocation, const Matrix4 &m);
        void setUniformVec3   (GLint uniformLocation, Vec3 &v);
        void setUniformVec4   (GLint uniformLocation, Vec4 &v);
        void setUniform1i(GLint uniformLocation, int i);

        void setDirectionToLight(Vec4 &v);

        void use();
        void dontUse();

    private:
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
