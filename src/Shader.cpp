#include "Shader.h"
#include "File.h"
#include "Matrix.h"
#include <cassert>

Shader::Shader(): program(0) { }

Shader::~Shader() {
    for (size_t i=0; i<shaders.size(); i++) {
        glDeleteShader(shaders[i]);
    }
}

GLint Shader::getUniformLocation(const char *name) const
{
    return glGetUniformLocation(program, name);
}

void Shader::setUniformMatrix3(GLint u, Matrix3 &m)
{
    assert(program != 0);
    // GLint u = glGetUniformLocation(program, name);
    // assert(u != -1);
    glUniformMatrix3fv(u, 1, GL_FALSE, m.value_ptr());
}

void Shader::setUniformMatrix4(GLint u, Matrix4 &m)
{
    assert(program != 0);
    //GLint u = glGetUniformLocation(program, name);
    // assert(u != -1);
    glUniformMatrix4fv(u, 1, GL_FALSE, m.value_ptr());  
}


void Shader::setUniformVec3(GLint u, Vec3 &v)
{
    assert(program != 0);
    //GLint u = glGetUniformLocation(program, name);
    //assert(u != -1);
    glUniform3fv(u, 1, (GLfloat *)v.value_ptr());

}

void Shader::setUniformVec4(GLint u, Vec4 &v)
{
    assert(program != 0);
    //GLint u = glGetUniformLocation(program, name);   
    //assert(u != -1);
    glUniform4fv(u, 1, (GLfloat *)v.value_ptr());
}


void Shader::setUniform1i(GLint u, int i)
{
    assert(program != 0);
    glUniform1i(u, i);
}

void Shader::setDirectionToLight(Vec4 &v) {
    assert(program != 0);
    GLint u          = glGetUniformLocation(program, "directionToLight");
    glUniform3fv(u, 1, (GLfloat *)v.value_ptr());
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::dontUse() {
    glUseProgram(0);
}

void Shader::done() {
    program = glCreateProgram();

    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "diffuseColor");
    glBindAttribLocation(program, 2, "normal");
    glBindAttribLocation(program, 3, "texture");

    CHECK_OPENGL_ERRORS(__LINE__);
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

    CHECK_OPENGL_ERRORS(__LINE__);
}

void Shader::addVertexShader(const char *path)
{
    CHECK_OPENGL_ERRORS(__LINE__);
    GLuint shader = addShader(GL_VERTEX_SHADER, path);
    shaders.push_back(shader);
    CHECK_OPENGL_ERRORS(__LINE__)
}

void Shader::addFragmentShader(const char *path)
{
    CHECK_OPENGL_ERRORS(__LINE__);
    GLuint shader = addShader(GL_FRAGMENT_SHADER, path);
    shaders.push_back(shader);
    CHECK_OPENGL_ERRORS(__LINE__)
}

GLuint Shader::addShader(GLenum type, const char *path)
{

    std::string src = File::readText(path);
    int srcLength = src.size();
    const char *srcBuffer = src.c_str();

    GLuint shader = glCreateShader(type);
    assert(shader != 0);
    CHECK_OPENGL_ERRORS(__LINE__);

    glShaderSource(shader, 1, (const char**)&srcBuffer, &srcLength);
    CHECK_OPENGL_ERRORS(__LINE__);

    glCompileShader(shader);
    CHECK_OPENGL_ERRORS(__LINE__);

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
