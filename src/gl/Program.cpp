#include "gl/Program.hpp"

using namespace gnev::gl;

Program::Program()
    : Handler(createHandle(), &deleteHandle) {}

Program::~Program() {}

void Program::glAttachShader(const Shader& shader) {
    Ctx::Get().glAttachShader(handle(), shader.handle());
}

void Program::glValidateProgram() { Ctx::Get().glValidateProgram(handle()); }

void Program::glLinkProgram() { Ctx::Get().glLinkProgram(handle()); }

void Program::glUseProgram() const { Ctx::Get().glUseProgram(handle()); }

void Program::glGetProgramiv(GLenum pname, GLint* params) const {
    Ctx::Get().glGetProgramiv(handle(), pname, params);
}

void Program::glGetProgramInfoLog(GLsizei bufSize,
                                  GLsizei* length,
                                  GLchar* infoLog) const {
    Ctx::Get().glGetProgramInfoLog(handle(), bufSize, length, infoLog);
}

GLint Program::glGetUniformBlockIndex(const GLchar* uniformBlockName) const {
    return Ctx::Get().glGetUniformBlockIndex(handle(), uniformBlockName);
}

void Program::glUniformBlockBinding(GLuint uniformBlockIndex,
                                    GLuint uniformBlockBinding) const {
    Ctx::Get().glUniformBlockBinding(handle(), uniformBlockIndex, uniformBlockBinding);
}

GLint Program::glGetProgramResourceIndex(GLenum programInterface,
                                         const GLchar* name) const {
    return Ctx::Get().glGetProgramResourceIndex(handle(), programInterface, name);
}

void Program::glShaderStorageBlockBinding(GLuint storageBlockIndex,
                                          GLuint storageBlockBinding) const {
    Ctx::Get().glShaderStorageBlockBinding(handle(),
                                           storageBlockIndex,
                                           storageBlockBinding);
}

GLint Program::glGetAttribLocation(const GLchar* name) const {
    return Ctx::Get().glGetAttribLocation(handle(), name);
}

GLint Program::glGetUniformLocation(const GLchar* name) const {
    return Ctx::Get().glGetUniformLocation(handle(), name);
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}
