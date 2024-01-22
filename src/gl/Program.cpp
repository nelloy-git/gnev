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
    L2()->log();
}

Program::~Program() { L2()->log(); }

void Program::attach(const Shader& shader) {
    Ctx::Get().glAttachShader(handle(), shader.handle());
    L2()->log(shader.handle());
}

void Program::validate() {
    Ctx::Get().glValidateProgram(handle());
    L2()->log();
}

bool Program::isValidateSucceed() const {
    GLint is_valid;
    Ctx::Get().glGetProgramiv(handle(), GL_VALIDATE_STATUS, &is_valid);
    L2()->logRes(is_valid == GL_TRUE);
    return is_valid == GL_TRUE;
}

void Program::link() {
    Ctx::Get().glLinkProgram(handle());
    L2()->log();
}

bool Program::isLinkSucceed() const {
    GLint is_linked;
    Ctx::Get().glGetProgramiv(handle(), GL_LINK_STATUS, &is_linked);
    L2()->logRes(is_linked == GL_TRUE);
    return is_linked == GL_TRUE;
}

void Program::use() const {
    Ctx::Get().glUseProgram(handle());
    L2()->log();
}

GLsizei Program::getInfoLogLength() const {
    GLsizei len;
    Ctx::Get().glGetProgramiv(handle(),
                              GL_INFO_LOG_LENGTH,
                              reinterpret_cast<GLint*>(&len));
    L2()->log(len);
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

    L2()->logRes(fmt::CharPtr{info_log.c_str()});
    return info_log;
}

GLint Program::getAttributeLoc(const GLchar* name) const {
    GLint index = Ctx::Get().glGetAttribLocation(handle(), name);
    L2()->logRes(fmt::CharPtr{name}, index);
    return index;
}

GLint Program::getUniformLoc(const GLchar* name) const {
    GLint index = Ctx::Get().glGetUniformLocation(handle(), name);
    L2()->logRes(fmt::CharPtr{name}, index);
    return index;
}

GLint Program::getResourceIndex(GLenum interface, const GLchar* name) const {
    GLint index = Ctx::Get().glGetProgramResourceIndex(handle(), interface, name);
    L2()->logRes(fmt::Enum{interface, fmt::Enum::Group::GetProgramResourceIndex},
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
    L2()->log(storage_block_index, buffer->handle());
    std::optional<GLuint> binding_index_opt =
        shader_storage_blocks->set(storage_block_index, buffer.getPtr());
    if (not binding_index_opt) {
        Log::ERROR("No free binds left");
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
    L2()->log(uniform_block_index, buffer->handle());
    std::optional<GLuint> binding_index_opt =
        shader_uniform_blocks->set(uniform_block_index, buffer.getPtr());
    if (not binding_index_opt) {
        Log::ERROR("No free binds left");
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
    L2()->log(texture_sampler_index, texture->handle());
    std::optional<GLuint> binding_index_opt =
        shader_texture_samplers->set(texture_sampler_index, texture.getPtr());
    if (not binding_index_opt) {
        Log::ERROR("No free binds left");
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
    L2()->logRes(MAX_SHADER_STORAGE_BUFFER_BINDINGS);
    return MAX_SHADER_STORAGE_BUFFER_BINDINGS;
}

GLuint Program::getMaxUniformBufferBindings() {
    static GLuint MAX_UNIFORM_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
        return value;
    }();
    L2()->logRes(MAX_UNIFORM_BUFFER_BINDINGS);
    return MAX_UNIFORM_BUFFER_BINDINGS;
}

GLuint Program::getMaxTextureImageUnits() {
    static GLuint MAX_TEXTURE_IMAGE_UNITS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
        return value;
    }();
    L2()->logRes(MAX_TEXTURE_IMAGE_UNITS);
    return MAX_TEXTURE_IMAGE_UNITS;
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}
