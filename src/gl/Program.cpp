#include "gl/Program.hpp"

using namespace gnev::gl;

Program::Program(const Ctx& ctx)
    : Handler(ctx, createHandle(ctx), &freeHandle) {}

Program::~Program() {}

void Program::glAttachShader(const Shader& shader) {
    ctx().glAttachShader(handle(), shader.handle());
}

void Program::glValidateProgram() { ctx().glValidateProgram(handle()); }

void Program::glLinkProgram() { ctx().glLinkProgram(handle()); }

void Program::glUseProgram() const { ctx().glUseProgram(handle()); }

void Program::glGetProgramiv(GLenum pname, GLint* params) const {
    ctx().glGetProgramiv(handle(), pname, params);
}

void Program::glGetProgramInfoLog(GLsizei bufSize,
                                  GLsizei* length,
                                  GLchar* infoLog) const {
    ctx().glGetProgramInfoLog(handle(), bufSize, length, infoLog);
}

GLint Program::glGetUniformBlockIndex(const GLchar* uniformBlockName) const {
    return ctx().glGetUniformBlockIndex(handle(), uniformBlockName);
}

void Program::glUniformBlockBinding(GLuint uniformBlockIndex,
                                    GLuint uniformBlockBinding) const {
    ctx().glUniformBlockBinding(handle(), uniformBlockIndex, uniformBlockBinding);
}

GLint Program::glGetProgramResourceIndex(GLenum programInterface,
                                         const GLchar* name) const {
    return ctx().glGetProgramResourceIndex(handle(), programInterface, name);
}

void Program::glShaderStorageBlockBinding(GLuint storageBlockIndex,
                                          GLuint storageBlockBinding) const {
    ctx().glShaderStorageBlockBinding(handle(), storageBlockIndex, storageBlockBinding);
}

GLint Program::glGetAttribLocation(const GLchar* name) const {
    return ctx().glGetAttribLocation(handle(), name);
}

GLint Program::glGetUniformLocation(const GLchar* name) const {
    return ctx().glGetUniformLocation(handle(), name);
}

GLuint Program::createHandle(const Ctx& ctx) { return ctx.glCreateProgram(); }

void Program::freeHandle(const Ctx& ctx, GLuint handle) { ctx.glDeleteProgram(handle); }
