#include "gl/Program.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/CharPtr.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerLog.hpp"
#include "gl/program/ProgramBinding.hpp"

using namespace gnev::gl;

Program::Program()
    : Handler(createHandle(), &deleteHandle)
    , shader_storage_blocks(std::make_unique<
                            ProgramBinding<Buffer>>(getMaxShaderStorageBufferBindings()))
    , shader_uniform_blocks(std::make_unique<
                            ProgramBinding<Buffer>>(getMaxUniformBufferBindings()))
    , shader_texture_samplers(std::make_unique<
                              ProgramBinding<Texture>>(getMaxTextureImageUnits())) {
    Log()->L2();
}

Program::~Program() { Log()->L2(); }

void Program::attach(const Shader& shader) {
    Ctx::Get().glAttachShader(handle(), shader.handle());
    Log()->L2(shader.handle());
}

void Program::validate() {
    Ctx::Get().glValidateProgram(handle());
    Log()->L2();
}

bool Program::isValidateSucceed() const {
    GLint is_valid;
    Ctx::Get().glGetProgramiv(handle(), GL_VALIDATE_STATUS, &is_valid);
    Log()->L2res(is_valid == GL_TRUE);
    return is_valid == GL_TRUE;
}

void Program::link() {
    Ctx::Get().glLinkProgram(handle());
    Log()->L2();
}

bool Program::isLinkSucceed() const {
    GLint is_linked;
    Ctx::Get().glGetProgramiv(handle(), GL_LINK_STATUS, &is_linked);
    Log()->L2res(is_linked == GL_TRUE);
    return is_linked == GL_TRUE;
}

void Program::use() const {
    Ctx::Get().glUseProgram(handle());
    Log()->L2();
}

GLsizei Program::getInfoLogLength() const {
    GLsizei len;
    Ctx::Get().glGetProgramiv(handle(),
                              GL_INFO_LOG_LENGTH,
                              reinterpret_cast<GLint*>(&len));
    Log()->L2(len);
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

    Log()->L2res(fmt::CharPtr{info_log.c_str()});
    return info_log;
}

GLint Program::getAttributeLoc(const GLchar* name) const {
    GLint index = Ctx::Get().glGetAttribLocation(handle(), name);
    Log()->L2res(fmt::CharPtr{name}, index);
    return index;
}

GLint Program::getUniformLoc(const GLchar* name) const {
    GLint index = Ctx::Get().glGetUniformLocation(handle(), name);
    Log()->L2res(fmt::CharPtr{name}, index);
    return index;
}

GLint Program::getResourceIndex(GLenum interface, const GLchar* name) const {
    GLint index = Ctx::Get().glGetProgramResourceIndex(handle(), interface, name);
    Log()->L2res(fmt::Enum{interface, fmt::Enum::Group::GetProgramResourceIndex},
                 fmt::CharPtr{name},
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
    Log()->L2(storage_block_index, buffer->handle());
    std::optional<GLuint> binding_index_opt =
        shader_storage_blocks->set(storage_block_index, buffer.getPtr());
    if (not binding_index_opt) {
        Log()->Error("No free binds left");
        return;
    }
    Ctx::Get().glShaderStorageBlockBinding(handle(),
                                           storage_block_index,
                                           binding_index_opt.value());
    Ctx::Get().glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
                                binding_index_opt.value(),
                                buffer->handle());
}

void Program::bindShaderUniformBlockBuffer(const GLchar* uniform_block_name,
                                           const Ref<Buffer>& buffer) {
    bindShaderUniformBlockBuffer(getResourceIndex(GL_UNIFORM_BLOCK, uniform_block_name),
                                 buffer);
}

void Program::bindShaderUniformBlockBuffer(GLuint uniform_block_index,
                                           const Ref<Buffer>& buffer) {
    Log()->L2(uniform_block_index, buffer->handle());
    std::optional<GLuint> binding_index_opt =
        shader_uniform_blocks->set(uniform_block_index, buffer.getPtr());
    if (not binding_index_opt) {
        Log()->Error("No free binds left");
        return;
    }

    Ctx::Get().glUniformBlockBinding(handle(),
                                     uniform_block_index,
                                     binding_index_opt.value());
    Ctx::Get().glBindBufferBase(GL_UNIFORM_BUFFER,
                                binding_index_opt.value(),
                                buffer->handle());
}

void Program::bindShaderTextureSampler(const GLchar* texture_sampler_name,
                                       const Ref<Texture>& texture) {
    bindShaderTextureSampler(getUniformLoc(texture_sampler_name), texture);
}

void Program::bindShaderTextureSampler(GLuint texture_sampler_index,
                                       const Ref<Texture>& texture) {
    Log()->L2(texture_sampler_index, texture->handle());
    std::optional<GLuint> binding_index_opt =
        shader_texture_samplers->set(texture_sampler_index, texture.getPtr());
    if (not binding_index_opt) {
        Log()->Error("No free binds left");
        return;
    }
    Ctx::Get().glActiveTexture(GL_TEXTURE0 + binding_index_opt.value());
    Ctx::Get().glUniform1i(texture_sampler_index, binding_index_opt.value());
    Ctx::Get().glBindTexture(GL_TEXTURE_2D_ARRAY, texture->handle());
}

GLuint Program::getMaxShaderStorageBufferBindings() {
    static GLuint MAX_SHADER_STORAGE_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
        return value;
    }();
    Log()->L2res(MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    return MAX_SHADER_STORAGE_BUFFER_BINDINGS;
}

GLuint Program::getMaxUniformBufferBindings() {
    static GLuint MAX_UNIFORM_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
        return value;
    }();
    Log()->L2res(MAX_UNIFORM_BUFFER_BINDINGS);
    return MAX_UNIFORM_BUFFER_BINDINGS;
}

GLuint Program::getMaxTextureImageUnits() {
    static GLuint MAX_TEXTURE_IMAGE_UNITS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
        return value;
    }();
    Log()->L2res(MAX_TEXTURE_IMAGE_UNITS);
    return MAX_TEXTURE_IMAGE_UNITS;
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}
