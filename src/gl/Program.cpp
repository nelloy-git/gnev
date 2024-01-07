#include "gl/Program.hpp"

#include <iostream>

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

Program::Program()
    : Handler(createHandle(), &deleteHandle)
    , shader_storage_blocks(std::make_unique<
                            Bindings<Buffer>>(getMaxShaderStorageBufferBindings()))
    , shader_uniform_blocks(std::make_unique<
                            Bindings<Buffer>>(getMaxUniformBufferBindings()))
    , shader_texture_samplers(std::make_unique<
                              Bindings<Texture>>(getMaxTextureImageUnits())) {
    GNEV_TRACE_L2("Program_{}::ctor()", handle());
}

void Program::attach(const Shader& shader) {
    Ctx::Get().glAttachShader(handle(), shader.handle());
    GNEV_TRACE_L2("Program_{}::attach(Shader_{})", handle(), shader.handle());
}

void Program::validate() {
    Ctx::Get().glValidateProgram(handle());
    GNEV_TRACE_L2("Program_{}::validate()", handle());
}

bool Program::isValidateSucceed() const {
    GLint is_valid;
    Ctx::Get().glGetProgramiv(handle(), GL_VALIDATE_STATUS, &is_valid);
    GNEV_TRACE_L2("Program_{}::isValidateSucceed() -> {}", handle(), is_valid == GL_TRUE);
    return is_valid == GL_TRUE;
}

void Program::link() {
    Ctx::Get().glLinkProgram(handle());
    GNEV_TRACE_L2("Program_{}::link()", handle());
}

bool Program::isLinkSucceed() const {
    GLint is_linked;
    Ctx::Get().glGetProgramiv(handle(), GL_LINK_STATUS, &is_linked);
    GNEV_TRACE_L2("Program_{}::isLinkSucceed() -> {}", handle(), is_linked == GL_TRUE);
    return is_linked == GL_TRUE;
}

void Program::use() const {
    Ctx::Get().glUseProgram(handle());
    GNEV_TRACE_L2("Program_{}::use()", handle());
}

GLsizei Program::getInfoLogLength() const {
    GLsizei len;
    Ctx::Get().glGetProgramiv(handle(),
                              GL_INFO_LOG_LENGTH,
                              reinterpret_cast<GLint*>(&len));
    GNEV_TRACE_L2("Program_{}::getInfoLogLength() -> {}", handle(), len);
    return len;
}

std::string Program::getInfoLog() const {
    std::string info_log;
    GLsizei len;
    Ctx::Get().glGetProgramiv(handle(),
                              GL_INFO_LOG_LENGTH,
                              reinterpret_cast<GLint*>(&len));
    if (len > 0) {
        info_log.resize(len);
        Ctx::Get().glGetProgramInfoLog(handle(), len, &len, info_log.data());
    }

    GNEV_TRACE_L2("Program_{}::getInfoLog() -> \"{}\"", handle(), info_log.c_str());
    return info_log;
}

GLint Program::getAttributeLoc(const GLchar* name) const {
    GLint index = Ctx::Get().glGetAttribLocation(handle(), name);
    GNEV_TRACE_L2("Program_{}::getAttributeLoc(\"{}\") -> {}", handle(), name, index);
    return index;
}

GLint Program::getUniformLoc(const GLchar* name) const {
    GLint index = Ctx::Get().glGetUniformLocation(handle(), name);
    GNEV_TRACE_L2("Program_{}::getUniformLoc(\"{}\") -> {}", handle(), name, index);
    return index;
}

GLint Program::getResourceIndex(GLenum interface, const GLchar* name) const {
    GLint index = Ctx::Get().glGetProgramResourceIndex(handle(), interface, name);
    GNEV_TRACE_L2("Program_{}::getResourceIndex({}, \"{}\") -> {}",
                  handle(),
                  fmt::Enum{interface, fmt::Enum::Group::GetProgramResourceIndex},
                  name,
                  index);
    return index;
}

void Program::bindShaderStorageBlockBuffer(const GLchar* storage_block_name,
                                           const Ref<Buffer>& buffer) {
    bindShaderStorageBlockBuffer(getResourceIndex(GL_SHADER_STORAGE_BLOCK,
                                                  storage_block_name),
                                 buffer);
}

void Program::bindShaderStorageBlockBuffer(GLuint storage_block_index,
                                           const Ref<Buffer>& buffer) {
    auto iter = shader_storage_blocks->map.find(storage_block_index);
    if (iter != shader_storage_blocks->map.end()) {
        GLuint binding = iter->second.first;
        shader_storage_blocks->binds.freeIndex(binding);
    }

    auto binding_opt = shader_storage_blocks->binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_storage_blocks->map[storage_block_index] = {binding_opt.value(),
                                                       buffer.getPtr()};

    Ctx::Get().glShaderStorageBlockBinding(handle(),
                                           storage_block_index,
                                           binding_opt.value());
    Ctx::Get().glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
                                binding_opt.value(),
                                buffer->handle());
    GNEV_TRACE_L2("Program_{}::bindShaderStorageBlockBuffer({}, Buffer_{})",
                  handle(),
                  storage_block_index,
                  buffer->handle());
}

void Program::bindShaderUniformBlockBuffer(const GLchar* uniform_block_name,
                                           const Ref<Buffer>& buffer) {
    bindShaderUniformBlockBuffer(getResourceIndex(GL_UNIFORM_BLOCK, uniform_block_name),
                                 buffer);
}

void Program::bindShaderUniformBlockBuffer(GLuint uniform_block_index,
                                           const Ref<Buffer>& buffer) {
    auto iter = shader_uniform_blocks->map.find(uniform_block_index);
    if (iter != shader_uniform_blocks->map.end()) {
        GLuint binding = iter->second.first;
        shader_uniform_blocks->binds.freeIndex(binding);
    }

    auto binding_opt = shader_uniform_blocks->binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_uniform_blocks->map[uniform_block_index] = {binding_opt.value(),
                                                       buffer.getPtr()};

    Ctx::Get().glUniformBlockBinding(handle(), uniform_block_index, binding_opt.value());
    Ctx::Get().glBindBufferBase(GL_UNIFORM_BUFFER, binding_opt.value(), buffer->handle());
    GNEV_TRACE_L2("Program_{}::bindShaderUniformBlockBuffer({}, Buffer_{})",
                  handle(),
                  uniform_block_index,
                  buffer->handle());
}

void Program::bindShaderTextureSampler(const GLchar* texture_sampler_name,
                                       const Ref<Texture>& texture) {
    bindShaderTextureSampler(getUniformLoc(texture_sampler_name), texture);
}

void Program::bindShaderTextureSampler(GLuint texture_sampler_index,
                                       const Ref<Texture>& texture) {
    auto iter = shader_texture_samplers->map.find(texture_sampler_index);
    if (iter != shader_texture_samplers->map.end()) {
        GLuint binding = iter->second.first;
        shader_texture_samplers->binds.freeIndex(binding);
    }

    auto binding_opt = shader_texture_samplers->binds.useIndex();
    if (not binding_opt) {
        throw std::out_of_range("");
    }
    shader_texture_samplers->map[texture_sampler_index] = {binding_opt.value(),
                                                           texture.getPtr()};

    std::cout << "BindShaderTextureSampler: " << texture_sampler_index << " to "
              << binding_opt.value() << std::endl;
    Ctx::Get().glActiveTexture(GL_TEXTURE0 + binding_opt.value());
    Ctx::Get().glUniform1i(texture_sampler_index, binding_opt.value());
    Ctx::Get().glBindTexture(GL_TEXTURE_2D_ARRAY, texture->handle());
    GNEV_TRACE_L2("Program_{}::bindShaderTextureSampler({}, Texture_{})",
                  handle(),
                  texture_sampler_index,
                  texture->handle());
}

template <typename T>
Program::Bindings<T>::Bindings(GLuint capacity)
    : binds(capacity) {}

GLuint Program::getMaxShaderStorageBufferBindings() {
    static GLuint MAX_SHADER_STORAGE_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
        GNEV_TRACE_L2("GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS = {}", value);
        return value;
    }();
    return MAX_SHADER_STORAGE_BUFFER_BINDINGS;
}

GLuint Program::getMaxUniformBufferBindings() {
    static GLuint MAX_UNIFORM_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
        GNEV_TRACE_L2("GL_MAX_UNIFORM_BUFFER_BINDINGS = {}", value);
        return value;
    }();
    return MAX_UNIFORM_BUFFER_BINDINGS;
}

GLuint Program::getMaxTextureImageUnits() {
    static GLuint MAX_TEXTURE_IMAGE_UNITS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
        GNEV_TRACE_L2("GL_MAX_TEXTURE_IMAGE_UNITS = {}", value);
        return value;
    }();
    return MAX_TEXTURE_IMAGE_UNITS;
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}
