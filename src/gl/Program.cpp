#include "gl/Program.hpp"

using namespace gnev::gl;

Program::Program()
    : Handler(createHandle(), &deleteHandle) {}

void Program::glAttachShader(const Shader& shader) {
    Ctx::Get().glAttachShader(handle(), shader.handle());
}

void Program::glValidateProgram() { Ctx::Get().glValidateProgram(handle()); }

void Program::glLinkProgram() { Ctx::Get().glLinkProgram(handle()); }

void Program::use() const { Ctx::Get().glUseProgram(handle()); }

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

void Program::bindShaderStorage(const std::string& shaderStorageBlockName,
                                const Ref<Buffer>& buffer) {
    bindShaderStorage(glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK,
                                                shaderStorageBlockName.c_str()),
                      buffer);
}

void Program::bindShaderStorage(GLuint shaderStorageBlockIndex,
                                const Ref<Buffer>& buffer) {
    auto iter = shader_storages.map.find(shaderStorageBlockIndex);
    if (iter != shader_storages.map.end()) {
        GLuint binding = iter->second.first;
        shader_storages.binds.freeIndex(binding);
    }

    auto binding_opt = shader_storages.binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_storages.map[shaderStorageBlockIndex] = {binding_opt.value(), buffer.getPtr()};
    Ctx::Get().glShaderStorageBlockBinding(handle(),
                                           shaderStorageBlockIndex,
                                           binding_opt.value());
}

GLuint Program::getMaxShaderStorageBufferBindings() {
    static GLuint MAX_SHADER_STORAGE_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
        return value;
    }();
    return MAX_SHADER_STORAGE_BUFFER_BINDINGS;
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}
