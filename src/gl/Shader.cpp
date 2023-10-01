#include "gl/Shader.hpp"

using namespace gnev::gl;

Shader::Shader(const Ctx& ctx, GLenum type)
    : Handler(ctx, createHandle(ctx, type), &freeHandle) {}

Shader::~Shader() {}

void Shader::glShaderSource(GLsizei count,
                            const GLchar* const* string,
                            const GLint* length) {
    ctx().glShaderSource(handle(), count, string, length);
}

void Shader::glCompileShader() { ctx().glCompileShader(handle()); }

void Shader::glGetShaderiv(GLenum pname, GLint* params) const {
    ctx().glGetShaderiv(handle(), pname, params);
}

void Shader::glGetShaderInfoLog(GLsizei bufSize, GLsizei* length, GLchar* infoLog) const {
    ctx().glGetShaderInfoLog(handle(), bufSize, length, infoLog);
}

GLuint Shader::createHandle(const Ctx& ctx, GLenum type) {
    return ctx.glCreateShader(type);
}

void Shader::freeHandle(const Ctx& ctx, GLuint handle) { ctx.glDeleteShader(handle); }
