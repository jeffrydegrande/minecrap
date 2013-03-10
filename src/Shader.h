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

        void setUniformMatrix3(const char *name, Matrix3 &m);
        void setUniformMatrix4(const char *name, Matrix4 &m);
        void setUniformVec3(const char *name, Vec3 &v);
        void setUniformVec4(const char *name, Vec4 &v);

        void setCameraToClipMatrix(Matrix4 &m);
        void setModelToCameraMatrix(Matrix4 &m);
        void setNormalModelToCameraMatrix(Matrix3 &m);
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
