#include "Ctx.hpp"

#include <cstring>
#include <memory>
#include <string_view>

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/CharPtr.hpp"
#include "gl/fmt/Enum.hpp"
#include "util/Log.hpp"

#ifdef WIN32
#include <windows.h>
#endif

#ifdef GNEV_DO_NOT_LOG_FUNC_NAME
#define GNEV_CTX_LOG_L3_CALL(...)                                                        \
    GNEV_LOG_L3(GNEV_ARGS_STR(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)), ##__VA_ARGS__)
#else
#define GNEV_CTX_LOG_L3_CALL(...)                                                        \
    GNEV_LOG_L3("{}" GNEV_ARGS_STR(BOOST_PP_VARIADIC_SIZE(__VA_ARGS__)),                 \
                GNEV_GET_FUNC_NAME,                                                      \
                ##__VA_ARGS__)
#endif

#define GNEV_CTX_LOG_L3_CALL_HELPER(z, i, _) ({})

using enum gnev::LogLevel;

namespace gnev {

#ifdef WIN32

namespace {

void freeTlsCtx(unsigned long* tls_index) {
    auto tls_ptr = TlsGetValue(*tls_index);
    if (tls_ptr == 0) {
        throw std::runtime_error("TlsGetValue error");
    }

    auto ctx_ptr = static_cast<Ctx*>(tls_ptr);
    delete ctx_ptr;
    TlsSetValue(*tls_index, nullptr);

    TlsFree(*tls_index);
    delete tls_index;
}

} // namespace

std::unique_ptr<unsigned long, void (*)(unsigned long*)> Ctx::tls_index{nullptr, nullptr};

#else

thread_local std::unique_ptr<Ctx> Ctx::thread_ctx = nullptr;

#endif

void Ctx::Init(LoadFunc load_func, quill::Logger* quill_logger) {
#ifdef WIN32
    if (!tls_index) {
        auto index_value = TlsAlloc();
        if (index_value == TLS_OUT_OF_INDEXES) {
            throw std::runtime_error("TlsAlloc error");
        }

        tls_index = std::unique_ptr<unsigned long,
                                    void (*)(unsigned long*)>(new DWORD(index_value),
                                                              &freeTlsCtx);
    }

    auto current_tls_ptr = TlsGetValue(*tls_index);
    if (GetLastError() != ERROR_SUCCESS) {
        throw std::runtime_error("TlsGetValue error");
    }

    if (current_tls_ptr != 0) {
        throw std::runtime_error("Context already inited");
    }

    Ctx* p_ctx = new Ctx(load_func);
    bool saved = TlsSetValue(*tls_index, p_ctx);
    if (!saved) {
        throw std::runtime_error("TlsSetValue error");
    }
#else
    if (thread_ctx) {
        throw std::runtime_error("");
    }
    thread_ctx = std::unique_ptr<Ctx>(new Ctx(load_func));
    Ctx* p_ctx = thread_ctx.get();
#endif
    if (quill_logger) {
        QUILL_LOG_INFO(quill_logger, "Quill logger in use");
    }
}

bool Ctx::IsInited() {
#ifdef WIN32
    return tls_index && (TlsGetValue(*tls_index) != 0);
#else
    return thread_ctx.get();
#endif
}

Ctx& Ctx::Get() {
#ifdef WIN32
    auto tls_ptr = TlsGetValue(*tls_index);
    if (tls_ptr == 0) {
        throw std::runtime_error("TlsGetValue error");
    }

    return *static_cast<Ctx*>(tls_ptr);
#else
    if (!thread_ctx) {
        throw std::runtime_error("");
    }
    return *thread_ctx;
#endif
}

Ctx::Ctx(LoadFunc load_func)
    : glad(std::make_unique<GladGLContext>()) {
    gladLoadGLContext(glad.get(), load_func);
}

void Ctx::setQuillLogger(quill::Logger* logger) { quill_logger = logger; }

quill::Logger* Ctx::getQuillLogger() const { return quill_logger; }

void Ctx::glActiveTexture(GLenum texture) const {
    GNEV_CTX_LOG_L3_CALL(texture);
    glad->ActiveTexture(texture);
}

void Ctx::glClear(GLbitfield mask) const {
    GNEV_CTX_LOG_L3_CALL((fmt::BitFlags{mask, fmt::BitFlags::Group::glClear}));
    glad->Clear(mask);
}

void Ctx::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const {
    GNEV_CTX_LOG_L3_CALL(red, green, blue, alpha);
    glad->ClearColor(red, green, blue, alpha);
}

void Ctx::glGetIntegerv(GLenum pname, GLint* params) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{pname}, static_cast<void*>(params));
    glad->GetIntegerv(pname, params);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(params), *params);
}

void Ctx::glEnable(GLenum pname) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{pname});
    glad->Enable(pname);
}

void Ctx::glDebugMessageCallback(GLDEBUGPROC callback, const void* userParam) const {
    GNEV_CTX_LOG_L3_CALL(reinterpret_cast<void*>(callback), userParam);
    glad->DebugMessageCallback(callback, userParam);
}

void Ctx::glDebugMessageControl(GLenum source,
                                GLenum type,
                                GLenum severity,
                                GLsizei count,
                                const GLuint* ids,
                                GLboolean enabled) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{source},
                         fmt::Enum{type},
                         fmt::Enum{severity},
                         count,
                         static_cast<const void*>(ids),
                         enabled);
    glad->DebugMessageControl(source, type, severity, count, ids, enabled);
}

void Ctx::glDrawElements(GLenum mode,
                         GLsizei count,
                         GLenum type,
                         const void* indices) const {

    GNEV_CTX_LOG_L3_CALL((fmt::Enum{mode, fmt::Enum::Group::DrawElements}),
                         count,
                         fmt::Enum{type},
                         indices);
    glad->DrawElements(mode, count, type, indices);
}

void Ctx::glCreateBuffers(GLsizei n, GLuint* buffers) const {
    GNEV_CTX_LOG_L3_CALL(n, static_cast<const void*>(buffers));
    glad->CreateBuffers(n, buffers);
    GNEV_LOG_L3("\t{} -> {}",
                static_cast<void*>(buffers),
                std::vector(buffers, buffers + n));
}

void Ctx::glDeleteBuffers(GLsizei n, GLuint* buffers) const {
    GNEV_CTX_LOG_L3_CALL(n, static_cast<const void*>(buffers));
    GNEV_LOG_L3("\t{} -> {}",
                static_cast<void*>(buffers),
                std::vector(buffers, buffers + n));
    glad->DeleteBuffers(n, buffers);
}

void Ctx::glBindBuffer(GLenum target, GLuint buffer) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{target}, buffer);
    glad->BindBuffer(target, buffer);
}

void Ctx::glBindBufferBase(GLenum target, GLuint index, GLuint buffer) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{target}, index, buffer);
    glad->BindBufferBase(target, index, buffer);
}

void Ctx::glBindBufferRange(GLenum target,
                            GLuint index,
                            GLuint buffer,
                            GLintptr offset,
                            GLsizeiptr size) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{target}, index, buffer, offset, size);
    glad->BindBufferRange(target, index, buffer, offset, size);
}

void Ctx::glNamedBufferData(GLuint buffer,
                            GLsizeiptr size,
                            const void* data,
                            GLenum usage) const {
    GNEV_CTX_LOG_L3_CALL(buffer, size, data, fmt::Enum{usage});
    glad->NamedBufferData(buffer, size, data, usage);
}

void Ctx::glNamedBufferStorage(GLuint buffer,
                               GLsizeiptr size,
                               const void* data,
                               GLbitfield flags) const {
    GNEV_CTX_LOG_L3_CALL(buffer,
                         size,
                         data,
                         (fmt::BitFlags{flags, fmt::BitFlags::Group::glBufferStorage}));
    glad->NamedBufferStorage(buffer, size, data, flags);
}

void Ctx::glNamedBufferSubData(GLuint buffer,
                               GLintptr offset,
                               GLsizeiptr size,
                               const void* data) const {
    GNEV_CTX_LOG_L3_CALL(buffer, offset, size, data);
    glad->NamedBufferSubData(buffer, offset, size, data);
}

void Ctx::glGetNamedBufferSubData(GLuint buffer,
                                  GLintptr offset,
                                  GLsizeiptr size,
                                  void* data) const {
    GNEV_CTX_LOG_L3_CALL(buffer, offset, size, data);
    glad->GetNamedBufferSubData(buffer, offset, size, data);
}

void Ctx::glCopyNamedBufferSubData(GLuint readBuffer,
                                   GLuint writeBuffer,
                                   GLintptr readOffset,
                                   GLintptr writeOffset,
                                   GLsizeiptr size) const {
    GNEV_CTX_LOG_L3_CALL(readBuffer, writeBuffer, readOffset, writeOffset, size);
    glad->CopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
}

void Ctx::glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint* params) const {
    GNEV_CTX_LOG_L3_CALL(buffer, fmt::Enum{pname}, static_cast<void*>(params));
    glad->GetNamedBufferParameteriv(buffer, pname, params);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(params), *params);
}

void* Ctx::glMapNamedBuffer(GLuint buffer, GLenum access) const {
    GNEV_CTX_LOG_L3_CALL(buffer, fmt::Enum{access});
    void* map = glad->MapNamedBuffer(buffer, access);
    GNEV_LOG_L3("\treturn {}", map);
    return map;
}

void* Ctx::glMapNamedBufferRange(GLuint buffer,
                                 GLintptr offset,
                                 GLsizeiptr length,
                                 GLbitfield access) const {
    GNEV_CTX_LOG_L3_CALL(buffer, offset, length, access);
    void* map = glad->MapNamedBufferRange(buffer, offset, length, access);
    GNEV_LOG_L3("\treturn {}", map);
    return map;
}

void Ctx::glFlushMappedNamedBufferRange(GLuint buffer,
                                        GLintptr offset,
                                        GLsizeiptr length) const {
    GNEV_CTX_LOG_L3_CALL(buffer, offset, length);
    glad->FlushMappedNamedBufferRange(buffer, offset, length);
}

void Ctx::glUnmapNamedBuffer(GLuint buffer) const {
    GNEV_CTX_LOG_L3_CALL(buffer);
    glad->UnmapNamedBuffer(buffer);
}

GLuint Ctx::glCreateShader(GLenum type) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{type});
    GLuint shader = glad->CreateShader(type);
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{type}, shader);
    return shader;
}

void Ctx::glDeleteShader(GLuint shader) const {
    GNEV_CTX_LOG_L3_CALL(shader);
    glad->DeleteShader(shader);
}

void Ctx::glShaderSource(GLuint shader,
                         GLsizei count,
                         const GLchar* const* string,
                         const GLint* length) const {
    GNEV_CTX_LOG_L3_CALL(shader,
                         count,
                         std::vector<std::string>(string, string + count),
                         std::vector(length, length + count));
    glad->ShaderSource(shader, count, string, length);
}

void Ctx::glCompileShader(GLuint shader) const {
    GNEV_CTX_LOG_L3_CALL(shader);
    glad->CompileShader(shader);
}

void Ctx::glGetShaderiv(GLuint shader, GLenum pname, GLint* params) const {
    GNEV_CTX_LOG_L3_CALL(shader, fmt::Enum{pname}, static_cast<void*>(params));
    glad->GetShaderiv(shader, pname, params);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(params), *params);
}

void Ctx::glGetShaderInfoLog(GLuint shader,
                             GLsizei bufSize,
                             GLsizei* length,
                             GLchar* infoLog) const {
    GNEV_CTX_LOG_L3_CALL(shader,
                         bufSize,
                         static_cast<void*>(length),
                         static_cast<void*>(infoLog));
    glad->GetShaderInfoLog(shader, bufSize, length, infoLog);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(length), *length);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(infoLog), fmt::CharPtr{infoLog});
}

GLuint Ctx::glCreateProgram() const {
    GNEV_CTX_LOG_L3_CALL();
    GLuint program = glad->CreateProgram();
    GNEV_LOG_L3("\treturn {}", program);
    return program;
}

void Ctx::glDeleteProgram(GLuint program) const {
    GNEV_CTX_LOG_L3_CALL(program);
    glad->DeleteProgram(program);
}

void Ctx::glAttachShader(GLuint program, GLuint shader) const {
    GNEV_CTX_LOG_L3_CALL(program, shader);
    glad->AttachShader(program, shader);
}

void Ctx::glValidateProgram(GLuint program) const {
    GNEV_CTX_LOG_L3_CALL(program);
    glad->ValidateProgram(program);
}

void Ctx::glLinkProgram(GLuint program) const {
    GNEV_CTX_LOG_L3_CALL(program);
    glad->LinkProgram(program);
}

void Ctx::glUseProgram(GLuint program) const {
    GNEV_CTX_LOG_L3_CALL(program);
    glad->UseProgram(program);
}

void Ctx::glGetProgramiv(GLuint program, GLenum pname, GLint* params) const {
    GNEV_CTX_LOG_L3_CALL(program, fmt::Enum{pname}, static_cast<const void*>(params));
    glad->GetProgramiv(program, pname, params);
    GNEV_LOG_L3("\t{} -> {}", static_cast<const void*>(params), *params);
}

void Ctx::glGetProgramInfoLog(GLuint program,
                              GLsizei bufSize,
                              GLsizei* length,
                              GLchar* infoLog) const {
    GNEV_CTX_LOG_L3_CALL(program,
                         bufSize,
                         static_cast<void*>(length),
                         static_cast<void*>(infoLog));
    glad->GetProgramInfoLog(program, bufSize, length, infoLog);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(length), *length);
    GNEV_LOG_L3("\t{} -> {}", static_cast<void*>(infoLog), fmt::CharPtr{infoLog});
}

void Ctx::glUniform1i(GLint location, GLint v0) const {
    GNEV_CTX_LOG_L3_CALL(location, v0);
    glad->Uniform1i(location, v0);
}

GLint Ctx::glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName) const {
    GNEV_CTX_LOG_L3_CALL(program, fmt::CharPtr{uniformBlockName});
    GLint index = glad->GetUniformBlockIndex(program, uniformBlockName);
    GNEV_CTX_LOG_L3_CALL(program, fmt::CharPtr{uniformBlockName}, index);
    return index;
}

void Ctx::glUniformBlockBinding(GLuint program,
                                GLuint uniformBlockIndex,
                                GLuint uniformBlockBinding) const {
    GNEV_CTX_LOG_L3_CALL(program, uniformBlockIndex, uniformBlockBinding);
    glad->UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

GLint Ctx::glGetProgramResourceIndex(GLuint program,
                                     GLenum programInterface,
                                     const GLchar* name) const {
    GNEV_CTX_LOG_L3_CALL(program, fmt::Enum{programInterface}, fmt::CharPtr{name});
    GLint index = glad->GetProgramResourceIndex(program, programInterface, name);
    GNEV_CTX_LOG_L3_CALL(program, fmt::Enum{programInterface}, fmt::CharPtr{name}, index);
    return index;
}

void Ctx::glShaderStorageBlockBinding(GLuint program,
                                      GLuint storageBlockIndex,
                                      GLuint storageBlockBinding) const {
    GNEV_CTX_LOG_L3_CALL(program, storageBlockIndex, storageBlockBinding);
    glad->ShaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding);
}

GLint Ctx::glGetAttribLocation(GLuint program, const GLchar* name) const {
    GNEV_CTX_LOG_L3_CALL(program, fmt::CharPtr{name});
    GLint loc = glad->GetAttribLocation(program, name);
    GNEV_CTX_LOG_L3_CALL(program, fmt::CharPtr{name}, loc);
    return loc;
}

GLint Ctx::glGetUniformLocation(GLuint program, const GLchar* name) const {
    GNEV_CTX_LOG_L3_CALL(program, fmt::CharPtr{name});
    GLint loc = glad->GetUniformLocation(program, name);
    GNEV_LOG_L3("\treturn {}", loc);
    return loc;
}

void Ctx::glCreateTextures(GLenum target, GLsizei n, GLuint* textures) const {
    GNEV_CTX_LOG_L3_CALL(n, static_cast<void*>(textures));
    glad->CreateTextures(target, n, textures);
    GNEV_LOG_L3("\t{} -> {}",
                static_cast<void*>(textures),
                std::vector(textures, textures + n));
}

void Ctx::glDeleteTextures(GLsizei n, GLuint* textures) const {
    GNEV_CTX_LOG_L3_CALL(n, std::vector(textures, textures + n));
    glad->DeleteTextures(n, textures);
}

void Ctx::glBindTexture(GLenum target, GLuint texture) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{target}, texture);
    glad->BindTexture(target, texture);
}

void Ctx::glTextureParameteri(GLuint texture, GLenum pname, GLint param) const {
    GNEV_CTX_LOG_L3_CALL(texture, fmt::Enum{pname}, param);
    glad->TextureParameteri(texture, pname, param);
}

void Ctx::glGetTextureParameteriv(GLuint texture, GLenum pname, GLint* param) const {
    GNEV_CTX_LOG_L3_CALL(texture, fmt::Enum{pname}, static_cast<const void*>(param));
    glad->GetTextureParameteriv(texture, pname, param);
    GNEV_LOG_L3("\t{} -> {}", static_cast<const void*>(param), *param);
}

void Ctx::glGetTextureLevelParameteriv(GLuint texture,
                                       GLint level,
                                       GLenum pname,
                                       GLint* param) const {
    GNEV_CTX_LOG_L3_CALL(texture,
                         level,
                         fmt::Enum{pname},
                         static_cast<const void*>(param));
    glad->GetTextureLevelParameteriv(texture, level, pname, param);
    GNEV_LOG_L3("\t{} -> {}", static_cast<const void*>(param), *param);
}

void Ctx::glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat* param) const {
    GNEV_CTX_LOG_L3_CALL(texture, fmt::Enum{pname}, static_cast<const void*>(param));
    glad->TextureParameterfv(texture, pname, param);
    GNEV_LOG_L3("\t{} -> {}", static_cast<const void*>(param), param[0]);
}

void Ctx::glTexImage3D(GLenum target,
                       GLint level,
                       GLint internalformat,
                       GLsizei width,
                       GLsizei height,
                       GLsizei depth,
                       GLint border,
                       GLenum format,
                       GLenum type,
                       const void* pixels) const {
    GNEV_CTX_LOG_L3_CALL(fmt::Enum{target},
                         level,
                         internalformat,
                         width,
                         height,
                         depth,
                         border,
                         fmt::Enum{format},
                         fmt::Enum{type},
                         pixels);
    glad->TexImage3D(target,
                     level,
                     internalformat,
                     width,
                     height,
                     depth,
                     border,
                     format,
                     type,
                     pixels);
}

void Ctx::glTextureStorage3D(GLuint texture,
                             GLsizei levels,
                             GLenum internalformat,
                             GLsizei width,
                             GLsizei height,
                             GLsizei depth) const {
    GNEV_CTX_LOG_L3_CALL(texture,
                         levels,
                         fmt::Enum{internalformat},
                         width,
                         height,
                         depth);
    glad->TextureStorage3D(texture, levels, internalformat, width, height, depth);
}

void Ctx::glTextureSubImage3D(GLuint texture,
                              GLint level,
                              GLint xoffset,
                              GLint yoffset,
                              GLint zoffset,
                              GLsizei width,
                              GLsizei height,
                              GLsizei depth,
                              GLenum format,
                              GLenum type,
                              const void* pixels) const {
    GNEV_CTX_LOG_L3_CALL(texture,
                         level,
                         xoffset,
                         yoffset,
                         zoffset,
                         width,
                         height,
                         depth,
                         fmt::Enum{format},
                         fmt::Enum{type},
                         pixels);
    glad->TextureSubImage3D(texture,
                            level,
                            xoffset,
                            yoffset,
                            zoffset,
                            width,
                            height,
                            depth,
                            format,
                            type,
                            pixels);
}

void Ctx::glGenerateTextureMipmap(GLuint texture) const {
    GNEV_CTX_LOG_L3_CALL(texture);
    glad->GenerateTextureMipmap(texture);
}

void Ctx::glCopyImageSubData(GLuint srcName,
                             GLenum srcTarget,
                             GLint srcLevel,
                             GLint srcX,
                             GLint srcY,
                             GLint srcZ,
                             GLuint dstName,
                             GLenum dstTarget,
                             GLint dstLevel,
                             GLint dstX,
                             GLint dstY,
                             GLint dstZ,
                             GLsizei srcWidth,
                             GLsizei srcHeight,
                             GLsizei srcDepth) const {
    GNEV_CTX_LOG_L3_CALL(srcName,
                         fmt::Enum{srcTarget},
                         srcLevel,
                         srcX,
                         srcY,
                         srcZ,
                         dstName,
                         fmt::Enum{dstTarget},
                         dstLevel,
                         dstX,
                         dstY,
                         dstZ,
                         srcWidth,
                         srcHeight,
                         srcDepth);
    glad->CopyImageSubData(srcName,
                           srcTarget,
                           srcLevel,
                           srcX,
                           srcY,
                           srcZ,
                           dstName,
                           dstTarget,
                           dstLevel,
                           dstX,
                           dstY,
                           dstZ,
                           srcWidth,
                           srcHeight,
                           srcDepth);
}

void Ctx::glGetTextureSubImage(GLuint texture,
                               GLint level,
                               GLint xoffset,
                               GLint yoffset,
                               GLint zoffset,
                               GLsizei width,
                               GLsizei height,
                               GLsizei depth,
                               GLenum format,
                               GLenum type,
                               GLsizei bufSize,
                               void* pixels) const {
    GNEV_CTX_LOG_L3_CALL(texture,
                         level,
                         xoffset,
                         yoffset,
                         zoffset,
                         width,
                         height,
                         depth,
                         fmt::Enum{format},
                         fmt::Enum{type},
                         bufSize,
                         pixels);
    glad->GetTextureSubImage(texture,
                             level,
                             xoffset,
                             yoffset,
                             zoffset,
                             width,
                             height,
                             depth,
                             format,
                             type,
                             bufSize,
                             pixels);
}

void Ctx::glCreateSamplers(GLsizei n, GLuint* samplers) const {
    GNEV_CTX_LOG_L3_CALL(n, static_cast<const void*>(samplers));
    glad->CreateSamplers(n, samplers);
    GNEV_LOG_L3("\t{} -> {}",
                static_cast<const void*>(samplers),
                std::vector(samplers, samplers + n));
}

void Ctx::glDeleteSamplers(GLsizei n, GLuint* samplers) const {
    GNEV_CTX_LOG_L3_CALL(n, std::vector(samplers, samplers + n));
    glad->DeleteSamplers(n, samplers);
}

void Ctx::glBindSampler(GLuint unit, GLuint sampler) const {
    GNEV_CTX_LOG_L3_CALL(unit, sampler);
    glad->BindSampler(unit, sampler);
}

void Ctx::glSamplerParameteri(GLuint sampler, GLuint pname, GLint param) const {
    GNEV_CTX_LOG_L3_CALL(sampler, fmt::Enum{pname}, param);
    glad->SamplerParameteri(sampler, pname, param);
}

void Ctx::glSamplerParameterf(GLuint sampler, GLuint pname, GLfloat param) const {
    GNEV_CTX_LOG_L3_CALL(sampler, fmt::Enum{pname}, param);
    glad->SamplerParameterf(sampler, pname, param);
}

void Ctx::glSamplerParameterfv(GLuint sampler, GLuint pname, const GLfloat* param) const {
    GNEV_CTX_LOG_L3_CALL(sampler,
                         fmt::Enum{pname},
                         static_cast<const void*>(param),
                         *param);
    glad->SamplerParameterfv(sampler, pname, param);
    GNEV_LOG_L3("\t{} -> {}", static_cast<const void*>(param), param[0]);
}

void Ctx::glCreateVertexArrays(GLsizei n, GLuint* arrays) const {
    GNEV_CTX_LOG_L3_CALL(n, static_cast<const void*>(arrays));
    glad->CreateVertexArrays(n, arrays);
    GNEV_LOG_L3("\t{} -> {}",
                static_cast<const void*>(arrays),
                std::vector(arrays, arrays + n));
}

void Ctx::glDeleteVertexArrays(GLsizei n, GLuint* arrays) const {
    GNEV_CTX_LOG_L3_CALL(n, std::vector(arrays, arrays + n));
    glad->DeleteVertexArrays(n, arrays);
}

void Ctx::glBindVertexArray(GLuint vaobj) const {
    GNEV_CTX_LOG_L3_CALL(vaobj);
    glad->BindVertexArray(vaobj);
}

void Ctx::glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer) const {
    GNEV_CTX_LOG_L3_CALL(vaobj, buffer);
    glad->VertexArrayElementBuffer(vaobj, buffer);
}

void Ctx::glVertexArrayVertexBuffer(GLuint vaobj,
                                    GLuint bindingindex,
                                    GLuint buffer,
                                    GLintptr offset,
                                    GLsizei stride) const {
    GNEV_CTX_LOG_L3_CALL(vaobj, bindingindex, buffer, offset, stride);
    glad->VertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
}

void Ctx::glVertexArrayAttribBinding(GLuint vaobj,
                                     GLuint attribindex,
                                     GLuint bindingindex) const {
    GNEV_CTX_LOG_L3_CALL(vaobj, attribindex, bindingindex);
    glad->VertexArrayAttribBinding(vaobj, attribindex, bindingindex);
}

void Ctx::glVertexArrayAttribFormat(GLuint vaobj,
                                    GLuint attribindex,
                                    GLint size,
                                    GLenum type,
                                    GLboolean normalized,
                                    GLuint relativeoffset) const {
    GNEV_CTX_LOG_L3_CALL(vaobj,
                         attribindex,
                         size,
                         fmt::Enum{type},
                         normalized,
                         relativeoffset);
    glad->VertexArrayAttribFormat(vaobj,
                                  attribindex,
                                  size,
                                  type,
                                  normalized,
                                  relativeoffset);
}

void Ctx::glVertexArrayBindingDivisor(GLuint vaobj,
                                      GLuint bindingindex,
                                      GLuint divisor) const {
    GNEV_CTX_LOG_L3_CALL(vaobj, bindingindex, divisor);
    glad->VertexArrayBindingDivisor(vaobj, bindingindex, divisor);
}

void Ctx::glEnableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    GNEV_CTX_LOG_L3_CALL(vaobj, index);
    glad->EnableVertexArrayAttrib(vaobj, index);
}

void Ctx::glDisableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    GNEV_CTX_LOG_L3_CALL(vaobj, index);
    glad->DisableVertexArrayAttrib(vaobj, index);
}

} // namespace gnev

#undef GNEV_CTX_LOG_L3_CALL
#undef GNEV_CTX_LOG_L3_CALL_HELPER
