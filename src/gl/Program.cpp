#include "gl/Program.hpp"

#include <iostream>

using namespace gnev::gl;

Program::Program()
    : Handler(createHandle(), &deleteHandle)
    , shader_storage_blocks(getMaxShaderStorageBufferBindings())
    , shader_uniform_blocks(getMaxUniformBufferBindings())
    , shader_texture_samplers(getMaxTextureImageUnits()) {}

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

void Program::bindShaderStorageBlockBuffer(const std::string& storage_block_name,
                                           const Ref<Buffer>& buffer) {
    bindShaderStorageBlockBuffer(glGetProgramResourceIndex(GL_SHADER_STORAGE_BLOCK,
                                                           storage_block_name.c_str()),
                                 buffer);
}

void Program::bindShaderStorageBlockBuffer(GLuint storage_block_index,
                                           const Ref<Buffer>& buffer) {
    auto iter = shader_storage_blocks.map.find(storage_block_index);
    if (iter != shader_storage_blocks.map.end()) {
        GLuint binding = iter->second.first;
        shader_storage_blocks.binds.freeIndex(binding);
    }

    auto binding_opt = shader_storage_blocks.binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_storage_blocks.map[storage_block_index] = {binding_opt.value(),
                                                      buffer.getPtr()};

    std::cout << "BindShaderStorageBlockBuffer: " << storage_block_index << " to "
              << binding_opt.value() << std::endl;
    Ctx::Get().glShaderStorageBlockBinding(handle(),
                                           storage_block_index,
                                           binding_opt.value());
    Ctx::Get().glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
                                binding_opt.value(),
                                buffer->handle());
}

void Program::bindShaderUniformBlockBuffer(const std::string& uniform_block_name,
                                           const Ref<Buffer>& buffer) {
    bindShaderUniformBlockBuffer(glGetProgramResourceIndex(GL_UNIFORM_BLOCK,
                                                           uniform_block_name.c_str()),
                                 buffer);
}

void Program::bindShaderUniformBlockBuffer(GLuint uniform_block_index,
                                           const Ref<Buffer>& buffer) {
    auto iter = shader_uniform_blocks.map.find(uniform_block_index);
    if (iter != shader_uniform_blocks.map.end()) {
        GLuint binding = iter->second.first;
        shader_uniform_blocks.binds.freeIndex(binding);
    }

    auto binding_opt = shader_uniform_blocks.binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_uniform_blocks.map[uniform_block_index] = {binding_opt.value(),
                                                      buffer.getPtr()};

    std::cout << "BindShaderUniformBlockBuffer: " << uniform_block_index << " to "
              << binding_opt.value() << std::endl;
    Ctx::Get().glUniformBlockBinding(handle(), uniform_block_index, binding_opt.value());
    Ctx::Get().glBindBufferBase(GL_UNIFORM_BUFFER, binding_opt.value(), buffer->handle());
}

void Program::bindShaderTextureSampler(const std::string& texture_sampler_name,
                                       const Ref<Texture>& texture) {
    bindShaderTextureSampler(glGetUniformLocation(texture_sampler_name.c_str()), texture);
}

void Program::bindShaderTextureSampler(GLuint texture_sampler_index,
                                       const Ref<Texture>& texture) {
    auto iter = shader_texture_samplers.map.find(texture_sampler_index);
    if (iter != shader_texture_samplers.map.end()) {
        GLuint binding = iter->second.first;
        shader_texture_samplers.binds.freeIndex(binding);
    }

    auto binding_opt = shader_texture_samplers.binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_texture_samplers.map[texture_sampler_index] = {binding_opt.value(),
                                                          texture.getPtr()};

    std::cout << "BindShaderTextureSampler: " << texture_sampler_index << " to "
              << binding_opt.value() << std::endl;
    Ctx::Get().glActiveTexture(GL_TEXTURE0 + binding_opt.value());
    Ctx::Get().glUniform1i(texture_sampler_index, binding_opt.value());
    Ctx::Get().glBindTexture(GL_TEXTURE_2D_ARRAY, texture->handle());
}

template <typename T>
Program::Bindings<T>::Bindings(GLuint capacity)
    : binds(capacity) {}

GLuint Program::getMaxShaderStorageBufferBindings() {
    static GLuint MAX_SHADER_STORAGE_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
        return value;
    }();
    return MAX_SHADER_STORAGE_BUFFER_BINDINGS;
}

GLuint Program::getMaxUniformBufferBindings() {
    static GLuint MAX_UNIFORM_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
        return value;
    }();
    return MAX_UNIFORM_BUFFER_BINDINGS;
}

GLuint Program::getMaxTextureImageUnits() {
    static GLuint MAX_TEXTURE_IMAGE_UNITS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
        return value;
    }();
    return MAX_TEXTURE_IMAGE_UNITS;
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}
