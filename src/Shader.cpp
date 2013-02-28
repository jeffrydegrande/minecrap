#include "Shader.h"
#include "File.h"
#include "Matrix.h"
#include "Graphics.h"
#include <cassert>

Shader::Shader():
    program(0),
    perspectiveMatrixUniform(-1),
    cameraMatrixUniform(-1)
{

}

void Shader::setCameraMatrix(Matrix4 &m) {
    assert (program != 0);

    if (cameraMatrixUniform == -1) {
        cameraMatrixUniform = glGetUniformLocation(program, "cameraMatrix");
    }
    assert(cameraMatrixUniform != -1);

    use();
    glUniformMatrix4fv(cameraMatrixUniform, 1, GL_FALSE, m.value_ptr());
    dontUse();
}

void Shader::setPerspectiveMatrix(Matrix4 &m) {
    assert (program != 0);

    if (perspectiveMatrixUniform == -1) {
        perspectiveMatrixUniform = glGetUniformLocation(program, "perspectiveMatrix");
    }
    assert(perspectiveMatrixUniform != -1);

    use();
    glUniformMatrix4fv(perspectiveMatrixUniform, 1, GL_FALSE, m.value_ptr());
    dontUse();
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::dontUse() {
    glUseProgram(0);
}

void Shader::done() {
    program = glCreateProgram();
    for (std::vector<GLuint>::iterator it = shaders.begin();
                                       it != shaders.end(); it++)
    {
        glAttachShader(program, *it);
    }

   glLinkProgram(program);
}


void Shader::addVertexShader(const char *path)
{
    GLuint shader = addShader(GL_VERTEX_SHADER, path);
    shaders.push_back(shader);
}

void Shader::addFragmentShader(const char *path)
{
    GLuint shader = addShader(GL_FRAGMENT_SHADER, path);
    shaders.push_back(shader);
}

GLuint Shader::addShader(GLenum type, const char *path)
{
    std::string src = File::readText(path);
    int srcLength = src.size();
    const char *srcBuffer = src.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**)&srcBuffer, &srcLength);
    glCompileShader(shader);
    checkCompileStatus(shader);
    return shader;
}

void Shader::checkCompileStatus(GLuint shader)
{
    int result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(GL_FALSE == result) {
        printf("ERROR compiling shader\n");
        int length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
      if(length > 0) {
        // create a log of error messages
        char* errorLog = new char[length];
        int written = 0;
        glGetShaderInfoLog(shader, length, &written, errorLog);
        printf("Shader error log;\n%s\n", errorLog);
        delete [] errorLog;
      }
      exit (-1);
    }
}
