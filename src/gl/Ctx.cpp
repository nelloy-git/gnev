#include "gl/Ctx.hpp"

#include <cstring>
#include <memory>
#include <source_location>

#include "gl/fmt/Enum.hpp"
#include "gl/fmt/BitFlags.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

#ifdef WIN32

#include <windows.h>

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

void Ctx::Init(LoadFunc load_func) {
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

    bool saved = TlsSetValue(*tls_index, new Ctx(load_func));
    if (!saved) {
        throw std::runtime_error("TlsSetValue error");
    }

    gnev::Log::init();
    GNEV_INFO("Logger inited");
}

bool Ctx::IsInited() { return tls_index && (TlsGetValue(*tls_index) != 0); }

Ctx& Ctx::Get() {
    auto tls_ptr = TlsGetValue(*tls_index);
    if (tls_ptr == 0) {
        throw std::runtime_error("TlsGetValue error");
    }

    return *static_cast<Ctx*>(tls_ptr);
}

#else

thread_local std::unique_ptr<Ctx> Ctx::thread_ctx = nullptr;

void Ctx::Init(LoadFunc load_func) {
    if (thread_ctx) {
        throw std::runtime_error("");
    }
    thread_ctx = std::unique_ptr<Ctx>(new Ctx(load_func));
    gnev::Log::init();
}

bool Ctx::IsInited() { return thread_ctx.get(); }

Ctx& Ctx::Get() {
    if (!thread_ctx) {
        throw std::runtime_error("");
    }
    return *thread_ctx;
}
#endif

Ctx::Ctx(LoadFunc load_func)
    : glad(std::make_unique<GladGLContext>()) {
    gladLoadGLContext(glad.get(), load_func);
}

void Ctx::glActiveTexture(GLenum texture) const {
    GNEV_TRACE_L3("glActiveTexture({})", fmt::Enum{texture});
    glad->ActiveTexture(texture);
}

void Ctx::glClear(GLbitfield mask) const {
    GNEV_TRACE_L3("glClear({})", fmt::BitFlags{mask, fmt::BitFlags::Group::glClear});
    glad->Clear(mask);
}

void Ctx::glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const {
    GNEV_TRACE_L3("glClearColor({}, {}, {}, {})", red, green, blue, alpha);
    glad->ClearColor(red, green, blue, alpha);
}

void Ctx::glGetIntegerv(GLenum pname, GLint* params) const {
    GNEV_TRACE_L3("glGetIntegerv({}, {})", fmt::Enum{pname}, static_cast<void*>(params));
    glad->GetIntegerv(pname, params);
    GNEV_TRACE_L3("{} -> {}...", static_cast<void*>(params), *params);
}

void Ctx::glEnable(GLenum pname) const {
    GNEV_TRACE_L3("glEnable({})", fmt::Enum{pname});
    glad->Enable(pname);
}

void Ctx::glDebugMessageCallback(GLDEBUGPROC callback, const void* userParam) const {
    GNEV_TRACE_L3("glDebugMessageCallback({}, {})",
                  reinterpret_cast<const void*>(callback),
                  userParam);
    glad->DebugMessageCallback(callback, userParam);
}

void Ctx::glDebugMessageControl(GLenum source,
                                GLenum type,
                                GLenum severity,
                                GLsizei count,
                                const GLuint* ids,
                                GLboolean enabled) const {
    GNEV_TRACE_L3("glDebugMessageControl({}, {}, {}, {}, {} -> {}, {})",
                  fmt::Enum{source},
                  fmt::Enum{type},
                  fmt::Enum{severity},
                  count,
                  static_cast<const void*>(ids),
                  std::vector(ids, ids + count),
                  enabled);
    glad->DebugMessageControl(source, type, severity, count, ids, enabled);
}

void Ctx::glDrawElements(GLenum mode,
                         GLsizei count,
                         GLenum type,
                         const void* indices) const {
    GNEV_TRACE_L3("glDrawElements({}, {}, {}, {})",
                  fmt::Enum{mode, fmt::Enum::Group::DrawElements},
                  count,
                  type,
                  count,
                  indices);
    glad->DrawElements(mode, count, type, indices);
}

void Ctx::glCreateBuffers(GLsizei n, GLuint* buffers) const {
    GNEV_TRACE_L3("glCreateBuffers({}, {})", n, static_cast<const void*>(buffers));
    glad->CreateBuffers(n, buffers);
    GNEV_TRACE_L3("{} -> {}",
                  static_cast<const void*>(buffers),
                  std::vector(buffers, buffers + n));
}

void Ctx::glDeleteBuffers(GLsizei n, GLuint* buffers) const {
    GNEV_TRACE_L3("glDeleteBuffers({}, {} -> {})",
                  n,
                  static_cast<void*>(buffers),
                  std::vector(buffers, buffers + n));
    glad->DeleteBuffers(n, buffers);
}

void Ctx::glBindBuffer(GLenum target, GLuint buffer) const {
    GNEV_TRACE_L3("glBindBuffer({}, {})", fmt::Enum{target}, buffer);
    glad->BindBuffer(target, buffer);
}

void Ctx::glBindBufferBase(GLenum target, GLuint index, GLuint buffer) const {
    GNEV_TRACE_L3("glBindBufferBase({}, {}, {})", fmt::Enum{target}, index, buffer);
    glad->BindBufferBase(target, index, buffer);
}

void Ctx::glBindBufferRange(GLenum target,
                            GLuint index,
                            GLuint buffer,
                            GLintptr offset,
                            GLsizeiptr size) const {
    GNEV_TRACE_L3("glBindBufferRange({}, {}, {}, {}, {})",
                  fmt::Enum{target},
                  index,
                  buffer,
                  offset,
                  size);
    glad->BindBufferRange(target, index, buffer, offset, size);
}

void Ctx::glNamedBufferData(GLuint buffer,
                            GLsizeiptr size,
                            const void* data,
                            GLenum usage) const {
    GNEV_TRACE_L3("glNamedBufferData({}, {}, {}, {})",
                  buffer,
                  size,
                  data,
                  fmt::Enum{usage});
    glad->NamedBufferData(buffer, size, data, usage);
}

void Ctx::glNamedBufferStorage(GLuint buffer,
                               GLsizeiptr size,
                               const void* data,
                               GLbitfield flags) const {
    GNEV_TRACE_L3("glNamedBufferStorage({}, {}, {}, {})",
                  buffer,
                  size,
                  data,
                  fmt::BitFlags{flags, fmt::BitFlags::Group::glBufferStorage});
    glad->NamedBufferStorage(buffer, size, data, flags);
}

void Ctx::glNamedBufferSubData(GLuint buffer,
                               GLintptr offset,
                               GLsizeiptr size,
                               const void* data) const {
    GNEV_TRACE_L3("glNamedBufferSubData({}, {}, {}, {})", buffer, offset, size, data);
    glad->NamedBufferSubData(buffer, offset, size, data);
}

void Ctx::glGetNamedBufferSubData(GLuint buffer,
                                  GLintptr offset,
                                  GLsizeiptr size,
                                  void* data) const {
    GNEV_TRACE_L3("glGetNamedBufferSubData({}, {}, {}, {})", buffer, offset, size, data);
    glad->GetNamedBufferSubData(buffer, offset, size, data);
}

void Ctx::glCopyNamedBufferSubData(GLuint readBuffer,
                                   GLuint writeBuffer,
                                   GLintptr readOffset,
                                   GLintptr writeOffset,
                                   GLsizeiptr size) const {
    GNEV_TRACE_L3("glCopyNamedBufferSubData({}, {}, {}, {}, {})",
                  readBuffer,
                  writeBuffer,
                  readOffset,
                  writeOffset,
                  size);
    glad->CopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
}

void Ctx::glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint* params) const {
    GNEV_TRACE_L3("glGetNamedBufferParameteriv({}, {}, {})",
                  buffer,
                  fmt::Enum{pname},
                  static_cast<void*>(params));
    glad->GetNamedBufferParameteriv(buffer, pname, params);
    GNEV_TRACE_L3("{} -> {}...", static_cast<void*>(params), *params);
}

void* Ctx::glMapNamedBuffer(GLuint buffer, GLenum access) const {
    void* map = glad->MapNamedBuffer(buffer, access);
    GNEV_TRACE_L3("glMapNamedBuffer({}, {}) -> {}", buffer, fmt::Enum{access}, map);
    return map;
}

void* Ctx::glMapNamedBufferRange(GLuint buffer,
                                 GLintptr offset,
                                 GLsizeiptr length,
                                 GLbitfield access) const {
    void* map = glad->MapNamedBufferRange(buffer, offset, length, access);
    GNEV_TRACE_L3("glMapNamedBufferRange({}, {}, {}, {}) -> {}",
                  buffer,
                  offset,
                  length,
                  access,
                  map);
    return map;
}

void Ctx::glFlushMappedNamedBufferRange(GLuint buffer,
                                        GLintptr offset,
                                        GLsizeiptr length) const {
    GNEV_TRACE_L3("glFlushMappedNamedBufferRange({}, {}, {})", buffer, offset, length);
    glad->FlushMappedNamedBufferRange(buffer, offset, length);
}

void Ctx::glUnmapNamedBuffer(GLuint buffer) const {
    GNEV_TRACE_L3("glUnmapNamedBuffer({})", buffer);
    glad->UnmapNamedBuffer(buffer);
}

GLuint Ctx::glCreateShader(GLenum type) const {
    GLuint shader = glad->CreateShader(type);
    GNEV_TRACE_L3("glCreateShader({}) -> {}", fmt::Enum{type}, shader);
    return shader;
}

void Ctx::glDeleteShader(GLuint shader) const {
    GNEV_TRACE_L3("glDeleteShader({})", shader);
    glad->DeleteShader(shader);
}

void Ctx::glShaderSource(GLuint shader,
                         GLsizei count,
                         const GLchar* const* string,
                         const GLint* length) const {
    GNEV_TRACE_L3("glShaderSource({}, {}, {}, {})",
                  shader,
                  count,
                  std::vector<std::string>(string, string + count),
                  std::vector(length, length + count));
    glad->ShaderSource(shader, count, string, length);
}

void Ctx::glCompileShader(GLuint shader) const {
    GNEV_TRACE_L3("glCompileShader({})", shader);
    glad->CompileShader(shader);
}

void Ctx::glGetShaderiv(GLuint shader, GLenum pname, GLint* params) const {
    GNEV_TRACE_L3("glGetShaderiv({}, {}, {})",
                  shader,
                  fmt::Enum{pname},
                  static_cast<void*>(params));
    glad->GetShaderiv(shader, pname, params);
    GNEV_TRACE_L3("{} -> {}...", static_cast<void*>(params), *params);
}

void Ctx::glGetShaderInfoLog(GLuint shader,
                             GLsizei bufSize,
                             GLsizei* length,
                             GLchar* infoLog) const {
    GNEV_TRACE_L3("glGetShaderInfoLog({}, {}, {}, {})",
                  shader,
                  bufSize,
                  static_cast<void*>(length),
                  static_cast<void*>(infoLog));
    glad->GetShaderInfoLog(shader, bufSize, length, infoLog);
    GNEV_TRACE_L3("{} -> {}", static_cast<void*>(length), *length);
    GNEV_TRACE_L3("{} -> \"{}\"", static_cast<void*>(infoLog), infoLog);
}

GLuint Ctx::glCreateProgram() const {
    GLuint program = glad->CreateProgram();
    GNEV_TRACE_L3("glCreateProgram() -> {}", program);
    return program;
}

void Ctx::glDeleteProgram(GLuint program) const {
    GNEV_TRACE_L3("glDeleteProgram({})", program);
    glad->DeleteProgram(program);
}

void Ctx::glAttachShader(GLuint program, GLuint shader) const {
    GNEV_TRACE_L3("glAttachShader({}, {})", program, shader);
    glad->AttachShader(program, shader);
}

void Ctx::glValidateProgram(GLuint program) const {
    GNEV_TRACE_L3("glValidateProgram({})", program);
    glad->ValidateProgram(program);
}

void Ctx::glLinkProgram(GLuint program) const {
    GNEV_TRACE_L3("glLinkProgram({})", program);
    glad->LinkProgram(program);
}

void Ctx::glUseProgram(GLuint program) const {
    GNEV_TRACE_L3("glUseProgram({})", program);
    glad->UseProgram(program);
}

void Ctx::glGetProgramiv(GLuint program, GLenum pname, GLint* params) const {
    GNEV_TRACE_L3("glGetProgramiv({}, {}, {})",
                  program,
                  fmt::Enum{pname},
                  static_cast<const void*>(params));
    glad->GetProgramiv(program, pname, params);
    GNEV_TRACE_L3("{} -> {}...", static_cast<const void*>(params), *params);
}

void Ctx::glGetProgramInfoLog(GLuint program,
                              GLsizei bufSize,
                              GLsizei* length,
                              GLchar* infoLog) const {
    GNEV_TRACE_L3("glGetProgramInfoLog({}, {}, {}, {})",
                  program,
                  bufSize,
                  static_cast<void*>(length),
                  static_cast<void*>(infoLog));
    glad->GetProgramInfoLog(program, bufSize, length, infoLog);
    GNEV_TRACE_L3("{} -> {}", static_cast<void*>(length), *length);
    GNEV_TRACE_L3("{} -> \"{}\"", static_cast<void*>(infoLog), infoLog);
}

void Ctx::glUniform1i(GLint location, GLint v0) const {
    GNEV_TRACE_L3("glUniform1i({}, {})", location, v0);
    glad->Uniform1i(location, v0);
}

GLint Ctx::glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName) const {
    GLint index = glad->GetUniformBlockIndex(program, uniformBlockName);
    GNEV_TRACE_L3("glGetUniformBlockIndex({}, \"{}\") -> {}",
                  program,
                  uniformBlockName,
                  index);
    return index;
}

void Ctx::glUniformBlockBinding(GLuint program,
                                GLuint uniformBlockIndex,
                                GLuint uniformBlockBinding) const {
    GNEV_TRACE_L3("glUniformBlockBinding({}, {}, {})",
                  program,
                  uniformBlockIndex,
                  uniformBlockBinding);
    glad->UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

GLint Ctx::glGetProgramResourceIndex(GLuint program,
                                     GLenum programInterface,
                                     const GLchar* name) const {
    GLint index = glad->GetProgramResourceIndex(program, programInterface, name);
    GNEV_TRACE_L3("glGetProgramResourceIndex({}, {}, \"{}\") -> {}",
                  program,
                  fmt::Enum{programInterface},
                  name,
                  index);
    return index;
}

void Ctx::glShaderStorageBlockBinding(GLuint program,
                                      GLuint storageBlockIndex,
                                      GLuint storageBlockBinding) const {
    GNEV_TRACE_L3("glShaderStorageBlockBinding({}, {}, {})",
                  program,
                  storageBlockIndex,
                  storageBlockBinding);
    glad->ShaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding);
}

GLint Ctx::glGetAttribLocation(GLuint program, const GLchar* name) const {
    GLint loc = glad->GetAttribLocation(program, name);
    GNEV_TRACE_L3("glGetAttribLocation({}, \"{}\") -> {}", program, name, loc);
    return loc;
}

GLint Ctx::glGetUniformLocation(GLuint program, const GLchar* name) const {
    GLint loc = glad->GetUniformLocation(program, name);
    GNEV_TRACE_L3("glGetUniformLocation({}, \"{}\") -> {}", program, name, loc);
    return loc;
}

void Ctx::glCreateTextures(GLenum target, GLsizei n, GLuint* textures) const {
    GNEV_TRACE_L3("glCreateTextures({}, {})", n, static_cast<void*>(textures));
    glad->CreateTextures(target, n, textures);
    GNEV_TRACE_L3("{} -> {}",
                  static_cast<void*>(textures),
                  std::vector(textures, textures + n));
}

void Ctx::glDeleteTextures(GLsizei n, GLuint* textures) const {
    GNEV_TRACE_L3("glDeleteTextures({}, {})", n, std::vector(textures, textures + n));
    glad->DeleteTextures(n, textures);
}

void Ctx::glBindTexture(GLenum target, GLuint texture) const {
    GNEV_TRACE_L3("glBindTexture({}, {})", fmt::Enum{target}, texture);
    glad->BindTexture(target, texture);
}

void Ctx::glTextureParameteri(GLuint texture, GLenum pname, GLint param) const {
    GNEV_TRACE_L3("glTextureParameteri({}, {}, {})", texture, fmt::Enum{pname}, param);
    glad->TextureParameteri(texture, pname, param);
}

void Ctx::glGetTextureParameteriv(GLuint texture, GLenum pname, GLint* param) const {
    GNEV_TRACE_L3("glGetTextureParameteriv({}, {}, {})",
                  texture,
                  fmt::Enum{pname},
                  static_cast<const void*>(param));
    glad->GetTextureParameteriv(texture, pname, param);
    GNEV_TRACE_L3("{} -> {}...", static_cast<const void*>(param), param[0]);
}

void Ctx::glGetTextureLevelParameteriv(GLuint texture,
                                       GLint level,
                                       GLenum pname,
                                       GLint* param) const {
    GNEV_TRACE_L3("glGetTextureLevelParameteriv({}, {}, {}, {})",
                  texture,
                  level,
                  fmt::Enum{pname},
                  static_cast<const void*>(param));
    glad->GetTextureLevelParameteriv(texture, level, pname, param);
    GNEV_TRACE_L3("{} -> {}...", static_cast<const void*>(param), param[0]);
}

void Ctx::glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat* param) const {
    GNEV_TRACE_L3("glTextureParameterfv({}, {}, {})",
                  texture,
                  fmt::Enum{pname},
                  static_cast<const void*>(param));
    glad->TextureParameterfv(texture, pname, param);
    GNEV_TRACE_L3("{} -> {}...", static_cast<const void*>(param), param[0]);
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
    GNEV_TRACE_L3("glTexImage3D({}, {}, {}, {}, {}, {}, {}, {}, {}, {})",
                  fmt::Enum{target},
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
    GNEV_TRACE_L3("glTextureStorage3D({}, {}, {}, {}, {}, {})",
                  texture,
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
    GNEV_TRACE_L3("glTextureSubImage3D({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {})",
                  texture,
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
    GNEV_TRACE_L3("glGenerateTextureMipmap({})", texture);
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
    GNEV_TRACE_L3("glCopyImageSubData({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, "
                  "{}, "
                  "{}, {}, {})",
                  srcName,
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
    GNEV_TRACE_L3("glGetTextureSubImage({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, "
                  "{})",
                  texture,
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
    GNEV_TRACE_L3("glCreateTextures({}, {})", n, static_cast<const void*>(samplers));
    glad->CreateSamplers(n, samplers);
    GNEV_TRACE_L3("{} -> {}",
                  static_cast<const void*>(samplers),
                  std::vector(samplers, samplers + n));
}

void Ctx::glDeleteSamplers(GLsizei n, GLuint* samplers) const {
    GNEV_TRACE_L3("glDeleteSamplers({}, {})", n, std::vector(samplers, samplers + n));
    glad->DeleteSamplers(n, samplers);
}

void Ctx::glBindSampler(GLuint unit, GLuint sampler) const {
    GNEV_TRACE_L3("glBindSampler({}, {})", unit, sampler);
    glad->BindSampler(unit, sampler);
}

void Ctx::glSamplerParameteri(GLuint sampler, GLuint pname, GLint param) const {
    GNEV_TRACE_L3("glSamplerParameteri({}, {}, {})", sampler, fmt::Enum{pname}, param);
    glad->SamplerParameteri(sampler, pname, param);
}

void Ctx::glSamplerParameterf(GLuint sampler, GLuint pname, GLfloat param) const {
    GNEV_TRACE_L3("glSamplerParameterf({}, {}, {})", sampler, fmt::Enum{pname}, param);
    glad->SamplerParameterf(sampler, pname, param);
}

void Ctx::glSamplerParameterfv(GLuint sampler, GLuint pname, const GLfloat* param) const {
    GNEV_TRACE_L3("glSamplerParameterfv({}, {}, {}) -> {}...",
                  sampler,
                  fmt::Enum{pname},
                  static_cast<const void*>(param),
                  *param);
    glad->SamplerParameterfv(sampler, pname, param);
    GNEV_TRACE_L3("{} -> {}...", static_cast<const void*>(param), param[0]);
}

void Ctx::glCreateVertexArrays(GLsizei n, GLuint* arrays) const {
    GNEV_TRACE_L3("glCreateVertexArrays({}, {})", n, static_cast<const void*>(arrays));
    glad->CreateVertexArrays(n, arrays);
    GNEV_TRACE_L3("{} -> {}",
                  static_cast<const void*>(arrays),
                  std::vector(arrays, arrays + n));
}

void Ctx::glDeleteVertexArrays(GLsizei n, GLuint* arrays) const {
    GNEV_TRACE_L3("glDeleteVertexArrays({}, {})", n, std::vector(arrays, arrays + n));
    glad->DeleteVertexArrays(n, arrays);
}

void Ctx::glBindVertexArray(GLuint vaobj) const {
    GNEV_TRACE_L3("glBindVertexArray({})", vaobj);
    glad->BindVertexArray(vaobj);
}

void Ctx::glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer) const {
    GNEV_TRACE_L3("glVertexArrayElementBuffer({}, {})", vaobj, buffer);
    glad->VertexArrayElementBuffer(vaobj, buffer);
}

void Ctx::glVertexArrayVertexBuffer(GLuint vaobj,
                                    GLuint bindingindex,
                                    GLuint buffer,
                                    GLintptr offset,
                                    GLsizei stride) const {
    GNEV_TRACE_L3("glVertexArrayVertexBuffer({}, {}, {}, {}, {})",
                  vaobj,
                  bindingindex,
                  buffer,
                  offset,
                  stride);
    glad->VertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
}

void Ctx::glVertexArrayAttribBinding(GLuint vaobj,
                                     GLuint attribindex,
                                     GLuint bindingindex) const {
    GNEV_TRACE_L3("glVertexArrayAttribBinding({}, {}, {})",
                  vaobj,
                  attribindex,
                  bindingindex);
    glad->VertexArrayAttribBinding(vaobj, attribindex, bindingindex);
}

void Ctx::glVertexArrayAttribFormat(GLuint vaobj,
                                    GLuint attribindex,
                                    GLint size,
                                    GLenum type,
                                    GLboolean normalized,
                                    GLuint relativeoffset) const {
    GNEV_TRACE_L3("glVertexArrayAttribFormat({}, {}, {}, {}, {}, {})",
                  vaobj,
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
    GNEV_TRACE_L3("glVertexArrayBindingDivisor({}, {}, {})",
                  vaobj,
                  bindingindex,
                  divisor);
    glad->VertexArrayBindingDivisor(vaobj, bindingindex, divisor);
}

void Ctx::glEnableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    GNEV_TRACE_L3("glEnableVertexArrayAttrib({}, {})", vaobj, index);
    glad->EnableVertexArrayAttrib(vaobj, index);
}

void Ctx::glDisableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    GNEV_TRACE_L3("glDisableVertexArrayAttrib({}, {})", vaobj, index);
    glad->DisableVertexArrayAttrib(vaobj, index);
}