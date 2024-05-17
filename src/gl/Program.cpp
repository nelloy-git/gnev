#include "gl/Program.hpp"

#include "gl/fmt/Enum.hpp"
#include "gl/fmt/String.hpp"
// #include "gl/program/ProgramBinding.hpp"

using enum gnev::LogLevel;

namespace gnev::gl {

Program::Program()
    : Handler(createHandle(), &deleteHandle)
// , shader_storage_blocks(std::make_unique<
//                         ProgramBinding<Buffer>>(getMaxShaderStorageBufferBindings()))
// , shader_uniform_blocks(std::make_unique<
//                         ProgramBinding<Buffer>>(getMaxUniformBufferBindings()))
// , shader_texture_samplers(std::make_unique<
//                           ProgramBinding<Texture>>(getMaxTextureImageUnits()))
{
    GNEV_HANDLER_LOG(L2);
}

Program::~Program() { GNEV_HANDLER_LOG(L2); }

void Program::attach(const Shader& shader) {
    GNEV_HANDLER_LOG(L2, shader.handle());
    Ctx::Get().glAttachShader(handle(), shader.handle());
}

void Program::validate() {
    GNEV_HANDLER_LOG(L2);
    Ctx::Get().glValidateProgram(handle());
}

bool Program::isValidateSucceed() const {
    GNEV_HANDLER_LOG(L2);
    GLint is_valid;
    Ctx::Get().glGetProgramiv(handle(), GL_VALIDATE_STATUS, &is_valid);
    return is_valid == GL_TRUE;
}

void Program::link() {
    GNEV_HANDLER_LOG(L2);
    Ctx::Get().glLinkProgram(handle());
}

bool Program::isLinkSucceed() const {
    GNEV_HANDLER_LOG(L2);
    GLint is_linked;
    Ctx::Get().glGetProgramiv(handle(), GL_LINK_STATUS, &is_linked);
    return is_linked == GL_TRUE;
}

void Program::use() const {
    GNEV_HANDLER_LOG(L2);
    Ctx::Get().glUseProgram(handle());
}

GLsizei Program::getInfoLogLength() const {
    GNEV_HANDLER_LOG(L2);
    GLsizei len;
    Ctx::Get().glGetProgramiv(handle(),
                              GL_INFO_LOG_LENGTH,
                              reinterpret_cast<GLint*>(&len));
    return len;
}

std::string Program::getInfoLog() const {
    GNEV_HANDLER_LOG(L2);

    std::string info_log{};
    GLsizei len;
    Ctx::Get().glGetProgramiv(handle(),
                              GL_INFO_LOG_LENGTH,
                              reinterpret_cast<GLint*>(&len));
    if (len > 0) {
        info_log.resize(len);
        Ctx::Get().glGetProgramInfoLog(handle(), len, &len, info_log.data());
    }

    return info_log;
}

GLint Program::getAttributeLoc(const GLchar* name) const {
    GNEV_HANDLER_LOG(L2, fmt::String{name});
    GLint index = Ctx::Get().glGetAttribLocation(handle(), name);
    return index;
}

GLint Program::getUniformLoc(const GLchar* name) const {
    GNEV_HANDLER_LOG(L2, fmt::String{name});
    GLint index = Ctx::Get().glGetUniformLocation(handle(), name);
    return index;
}

GLint Program::getResourceIndex(GLenum interface, const GLchar* name) const {
    GNEV_HANDLER_LOG(L2,
                     (fmt::Enum{interface, fmt::Enum::Group::GetProgramResourceIndex}),
                     fmt::String{name});
    GLint index = Ctx::Get().glGetProgramResourceIndex(handle(), interface, name);
    return index;
}

void Program::bindShaderStorageBlockBuffer(const GLchar* storage_block_name,
                                           const std::shared_ptr<Buffer>& buffer) {
    GNEV_HANDLER_LOG(L2, fmt::String{storage_block_name}, buffer->handle());
    bindShaderStorageBlockBuffer(getResourceIndex(GL_SHADER_STORAGE_BLOCK,
                                                  storage_block_name),
                                 buffer);
}

void Program::bindShaderStorageBlockBuffer(GLuint storage_block_index,
                                           const std::shared_ptr<Buffer>& buffer) {
    GNEV_LOG_CRITICAL("WIP");
    // Log().Func(storage_block_index, buffer->handle());
    // std::optional<GLuint> binding_index_opt =
    //     shader_storage_blocks->set(storage_block_index, buffer);
    // if (not binding_index_opt) {
    //     Log().ERROR("No free binds left");
    //     return;
    // }
    // Ctx::Get().glShaderStorageBlockBinding(handle(),
    //                                        storage_block_index,
    //                                        binding_index_opt.value());
    // Ctx::Get().glBindBufferBase(GL_SHADER_STORAGE_BUFFER,
    //                             binding_index_opt.value(),
    //                             buffer->handle());
}

void Program::bindShaderUniformBlockBuffer(const GLchar* uniform_block_name,
                                           const std::shared_ptr<Buffer>& buffer) {
    GNEV_HANDLER_LOG(L2, fmt::String{uniform_block_name}, buffer->handle());
    bindShaderUniformBlockBuffer(getResourceIndex(GL_UNIFORM_BLOCK, uniform_block_name),
                                 buffer);
}

void Program::bindShaderUniformBlockBuffer(GLuint uniform_block_index,
                                           const std::shared_ptr<Buffer>& buffer) {
    GNEV_LOG_CRITICAL("WIP");
    // Log().Func(uniform_block_index, buffer->handle());
    // std::optional<GLuint> binding_index_opt =
    //     shader_uniform_blocks->set(uniform_block_index, buffer);
    // if (not binding_index_opt) {
    //     Log().ERROR("No free binds left");
    //     return;
    // }

    // Ctx::Get().glUniformBlockBinding(handle(),
    //                                  uniform_block_index,
    //                                  binding_index_opt.value());
    // Ctx::Get().glBindBufferBase(GL_UNIFORM_BUFFER,
    //                             binding_index_opt.value(),
    //                             buffer->handle());
}

void Program::bindShaderTextureSampler(const GLchar* texture_sampler_name,
                                       const std::shared_ptr<Texture>& texture) {
    GNEV_HANDLER_LOG(L2, fmt::String{texture_sampler_name}, texture->handle());
    bindShaderTextureSampler(getUniformLoc(texture_sampler_name), texture);
}

void Program::bindShaderTextureSampler(GLuint texture_sampler_index,
                                       const std::shared_ptr<Texture>& texture) {
    GNEV_LOG_CRITICAL("WIP");
    // Log().Func(texture_sampler_index, texture->handle());
    // std::optional<GLuint> binding_index_opt =
    //     shader_texture_samplers->set(texture_sampler_index, texture);
    // if (not binding_index_opt) {
    //     Log().ERROR("No free binds left");
    //     return;
    // }
    // Ctx::Get().glActiveTexture(GL_TEXTURE0 + binding_index_opt.value());
    // Ctx::Get().glUniform1i(texture_sampler_index, binding_index_opt.value());
    // Ctx::Get().glBindTexture(GL_TEXTURE_2D_ARRAY, texture->handle());
}

GLuint Program::getMaxShaderStorageBufferBindings() {
    GNEV_HANDLER_LOG(L2);
    static GLuint MAX_SHADER_STORAGE_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &value);
        GNEV_LOG_INFO("GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS: {}", value);
        return value;
    }();
    return MAX_SHADER_STORAGE_BUFFER_BINDINGS;
}

GLuint Program::getMaxUniformBufferBindings() {
    GNEV_HANDLER_LOG(L2);
    static GLuint MAX_UNIFORM_BUFFER_BINDINGS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &value);
        GNEV_LOG_INFO("GL_MAX_UNIFORM_BUFFER_BINDINGS: {}", value);
        return value;
    }();
    return MAX_UNIFORM_BUFFER_BINDINGS;
}

GLuint Program::getMaxTextureImageUnits() {
    GNEV_HANDLER_LOG(L2);
    static GLuint MAX_TEXTURE_IMAGE_UNITS = []() {
        GLint value;
        Ctx::Get().glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
        GNEV_LOG_INFO("GL_MAX_TEXTURE_IMAGE_UNITS: {}", value);
        return value;
    }();
    return MAX_TEXTURE_IMAGE_UNITS;
}

GLuint* Program::createHandle() { return new GLuint(Ctx::Get().glCreateProgram()); }

void Program::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteProgram(*handle);
    delete handle;
}

} // namespace gnev::gl