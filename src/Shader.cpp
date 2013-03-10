#include "Shader.h"
#include "File.h"
#include "Matrix.h"
#include <cassert>

#define ASSERT_NO_GL_ERROR  assert(GL_NO_ERROR == glGetError())

Shader::Shader(): program(0) { }

Shader::~Shader() {
    for (size_t i=0; i<shaders.size(); i++) {
        glDeleteShader(shaders[i]);
    }
}


void Shader::setUniformMatrix3(const char *name, Matrix3 &m)
{
    assert(program != -1);
    GLint u = glGetUniformLocation(program, name);
    assert(u != -1);
    glUniformMatrix3fv(u, 1, GL_FALSE, m.value_ptr());
}

void Shader::setUniformMatrix4(const char *name, Matrix4 &m)
{
    assert(program != -1);
    GLint u = glGetUniformLocation(program, name);
    assert(u != -1);
    glUniformMatrix4fv(u, 1, GL_FALSE, m.value_ptr());  
}


void Shader::setUniformVec3(const char *name, Vec3 &v)
{
    assert(program != -1);
    GLint u = glGetUniformLocation(program, name);
    assert(u != -1);
    glUniform3fv(u, 1, (GLfloat *)v.value_ptr());

}

void Shader::setUniformVec4(const char *name, Vec4 &v)
{
    assert(program != -1);
    GLint u = glGetUniformLocation(program, name);   
    assert(u != -1);
    glUniform4fv(u, 1, (GLfloat *)v.value_ptr());
}


void Shader::setModelToCameraMatrix(Matrix4 &m) {
    assert(program != 0);
    GLint modelToCameraMatrix       = glGetUniformLocation(program, "modelToCameraMatrix");
    glUniformMatrix4fv(modelToCameraMatrix, 1, GL_FALSE, m.value_ptr());
}

void Shader::setCameraToClipMatrix(Matrix4 &m) {
    assert(program != 0);   
    GLint v = glGetUniformLocation(program, "cameraToClipMatrix");
    glUniformMatrix4fv(v, 1, GL_FALSE, m.value_ptr());
}


void Shader::setDirectionToLight(Vec4 &v) {
    assert(program != 0);
    GLint u          = glGetUniformLocation(program, "directionToLight");
    glUniform3fv(u, 1, (GLfloat *)v.value_ptr());
}

void Shader::setNormalModelToCameraMatrix(Matrix3 &m) {
    assert(program != 0);
    GLint v = glGetUniformLocation(program, "normalModelToCameraMatrix");

    glUniformMatrix3fv(v, 1, GL_FALSE, m.value_ptr());
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::dontUse() {
    glUseProgram(0);
}

void Shader::done() {
    program = glCreateProgram();
    for (size_t i=0; i<shaders.size(); i++) {
        glAttachShader(program, shaders[i]);
    }

    glLinkProgram(program);
    //Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);

    if(isLinked == GL_FALSE) {
        printf("Can not link shader program\n");
        exit(1);
    }

    for (size_t i=0; i<shaders.size(); i++) {
        glDetachShader(program, shaders[i]);
    }

    ASSERT_NO_GL_ERROR;
}


void Shader::addVertexShader(const char *path)
{
    GLuint shader = addShader(GL_VERTEX_SHADER, path);
    shaders.push_back(shader);
    ASSERT_NO_GL_ERROR;
}

void Shader::addFragmentShader(const char *path)
{
    GLuint shader = addShader(GL_FRAGMENT_SHADER, path);
    shaders.push_back(shader);
    ASSERT_NO_GL_ERROR;
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
      glDeleteShader(shader);
      exit (-1);
    }
}
