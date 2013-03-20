#include "Shader.h"
#include "File.h"
#include "Matrix.h"
#include <cassert>

#define ASSERT_NO_GL_ERROR  assert(GL_NO_ERROR == glGetError())




#ifdef _WIN32
    const std::string ShadersDirectory("..\\shaders\\");
#else
    const std::string ShadersDirectory("./shaders/");
#endif

Shader::Shader(): program(0) { }

Shader::~Shader() {
    for (size_t i=0; i<shaders.size(); i++) {
        glDeleteShader(shaders[i]);
    }
}

GLint Shader::getUniformLocation(const char *name) {
    std::map<std::string, GLint>::iterator it = uniforms.find(name);

    GLint u = 0;

    if (it == uniforms.end()) {
        u = glGetUniformLocation(program, name);
        uniforms[name] = u;
    } else {
       u = it->second;
    }
//    assert(u > 0);
    return u;
}

void Shader::setUniform(const char *name, Matrix4 &m) 
{
    GLint u = getUniformLocation(name);
    glUniformMatrix4fv(u, 1, GL_FALSE, m.value_ptr());
}

void Shader::setUniform(const char *name, const Matrix4 &m)
{
    GLint u = getUniformLocation(name);
    glUniformMatrix4fv(u, 1, GL_FALSE, m.value_ptr());
}

void Shader::setUniform(const char *name, Matrix3 &m)
{
    GLint u = getUniformLocation(name);
    glUniformMatrix3fv(u, 1, GL_FALSE, m.value_ptr());
}

void Shader::setUniform(const char *name, Vec3 &v)
{
    GLint u = getUniformLocation(name);
    glUniform3fv(u, 1, (GLfloat *)v.value_ptr());

}

void Shader::setUniform(const char *name, Vec4 &v)
{
    GLint u = getUniformLocation(name);
    glUniform4fv(u, 1, (GLfloat *)v.value_ptr());
}


void Shader::setUniform(const char *name, int i)
{
    GLint u = getUniformLocation(name);
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
    std::string fullPath(ShadersDirectory);
    fullPath += path;
    fullPath += ".vert";

    printf("Loading vertex shader: %s\n", fullPath.c_str());
    GLuint shader = addShader(GL_VERTEX_SHADER, fullPath.c_str());
    shaders.push_back(shader);
    ASSERT_NO_GL_ERROR;
}

void Shader::addFragmentShader(const char *path)
{
    std::string fullPath(ShadersDirectory);
    fullPath += path;
    fullPath += ".frag";

    printf("Loading fragment shader: %s\n", fullPath.c_str());
    GLuint shader = addShader(GL_FRAGMENT_SHADER, fullPath.c_str());
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
