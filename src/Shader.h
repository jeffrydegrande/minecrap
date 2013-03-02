#ifndef MINECRAP_SHADER_H
#define MINECRAP_SHADER_H

#include "minecrap.h"
#include <vector>
#include "Matrix.h"
#include "Vec.h"

class Shader {
    GLuint program;

    GLint cameraToClipMatrix;
    GLint modelToCameraMatrix;
    GLint directionToLight;
    GLint normalModelToCameraMatrix;

    std::vector<GLuint> shaders;

    public:
        Shader();
        ~Shader();

        void addVertexShader(const char *path);
        void addFragmentShader(const char *path);
        void done();

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
