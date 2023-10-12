#include "gl/Shader.hpp"

using namespace gnev::gl;

Shader::Shader(GLenum type)
    : Handler(createHandle(type), &deleteHandle) {}

Shader::~Shader() {}

void Shader::glShaderSource(GLsizei count,
                            const GLchar* const* string,
                            const GLint* length) {
    Ctx::Get().glShaderSource(handle(), count, string, length);
}

void Shader::glCompileShader() { Ctx::Get().glCompileShader(handle()); }

void Shader::glGetShaderiv(GLenum pname, GLint* params) const {
    Ctx::Get().glGetShaderiv(handle(), pname, params);
}

void Shader::glGetShaderInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog) const {
    Ctx::Get().glGetShaderInfoLog(handle(), bufSize, length, infoLog);
}

GLuint* Shader::createHandle(GLenum type) {
    return new GLuint(Ctx::Get().glCreateShader(type));
}

void Shader::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteShader(*handle);
    delete handle;
}
