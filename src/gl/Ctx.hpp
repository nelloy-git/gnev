#pragma once

#include <memory>
#include <stdexcept>

#include "gl/logger/CtxLogger.hpp"
#include "glad/gl.h"
#include "util/Export.hpp"
#include "util/Logger.hpp"
#include "util/SrcLoc.hpp"

struct GladGLContext;

namespace gnev::gl {

class CtxLogger;

class EXPORT Ctx {
public:
    using ApiProc = void (*)(void);
    using LoadFunc = ApiProc (*)(const char*);

    static void Init(LoadFunc load_func, quill::Logger* quill_logger);
    static bool IsInited();
    static Ctx& Get();
    virtual ~Ctx() = default;

public:
    CtxLogger getLogger(const SrcLoc& src_loc = SrcLoc::Current()) const;

    // Global

    void glActiveTexture(GLenum texture) const;
    void glClear(GLbitfield mask) const;
    void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const;
    void glGetIntegerv(GLenum pname, GLint* params) const;
    void glEnable(GLenum pname) const;
    void glDebugMessageCallback(GLDEBUGPROC callback, const void* userParam) const;
    void glDebugMessageControl(GLenum source,
                               GLenum type,
                               GLenum severity,
                               GLsizei count,
                               const GLuint* ids,
                               GLboolean enabled) const;

    // Draw

    void
    glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) const;

    // Buffer

    void glCreateBuffers(GLsizei n, GLuint* buffers) const;
    void glDeleteBuffers(GLsizei n, GLuint* buffers) const;
    void glBindBuffer(GLenum target, GLuint buffer) const;
    void glBindBufferBase(GLenum target, GLuint index, GLuint buffer) const;
    void glBindBufferRange(GLenum target,
                           GLuint index,
                           GLuint buffer,
                           GLintptr offset,
                           GLsizeiptr size) const;
    void glNamedBufferData(GLuint buffer,
                           GLsizeiptr size,
                           const void* data,
                           GLenum usage) const;
    void glNamedBufferStorage(GLuint buffer,
                              GLsizeiptr size,
                              const void* data,
                              GLbitfield flags) const;
    void glNamedBufferSubData(GLuint buffer,
                              GLintptr offset,
                              GLsizeiptr size,
                              const void* data) const;
    void glGetNamedBufferSubData(GLuint buffer,
                                 GLintptr offset,
                                 GLsizeiptr size,
                                 void* data) const;
    void glCopyNamedBufferSubData(GLuint readBuffer,
                                  GLuint writeBuffer,
                                  GLintptr readOffset,
                                  GLintptr writeOffset,
                                  GLsizeiptr size) const;
    void glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint* params) const;
    void* glMapNamedBuffer(GLuint buffer, GLenum access) const;
    void* glMapNamedBufferRange(GLuint buffer,
                                GLintptr offset,
                                GLsizeiptr length,
                                GLbitfield access) const;
    void glFlushMappedNamedBufferRange(GLuint buffer,
                                       GLintptr offset,
                                       GLsizeiptr length) const;
    void glUnmapNamedBuffer(GLuint buffer) const;

    // Shader

    GLuint glCreateShader(GLenum type) const;
    void glDeleteShader(GLuint shader) const;
    void glShaderSource(GLuint shader,
                        GLsizei count,
                        const GLchar* const* string,
                        const GLint* length) const;
    void glCompileShader(GLuint shader) const;
    void glGetShaderiv(GLuint shader, GLenum pname, GLint* params) const;
    void glGetShaderInfoLog(GLuint shader,
                            GLsizei bufSize,
                            GLsizei* length,
                            GLchar* infoLog) const;

    // Program

    GLuint glCreateProgram() const;
    void glDeleteProgram(GLuint program) const;
    void glAttachShader(GLuint program, GLuint shader) const;
    void glValidateProgram(GLuint program) const;
    void glLinkProgram(GLuint program) const;
    void glUseProgram(GLuint program) const;
    void glGetProgramiv(GLuint program, GLenum pname, GLint* params) const;
    void glGetProgramInfoLog(GLuint program,
                             GLsizei bufSize,
                             GLsizei* length,
                             GLchar* infoLog) const;
    void glUniform1i(GLint location, GLint v0) const;
    GLint glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName) const;
    void glUniformBlockBinding(GLuint program,
                               GLuint uniformBlockIndex,
                               GLuint uniformBlockBinding) const;
    GLint glGetProgramResourceIndex(GLuint program,
                                    GLenum programInterface,
                                    const GLchar* name) const;
    void glShaderStorageBlockBinding(GLuint program,
                                     GLuint storageBlockIndex,
                                     GLuint storageBlockBinding) const;
    GLint glGetAttribLocation(GLuint program, const GLchar* name) const;
    GLint glGetUniformLocation(GLuint program, const GLchar* name) const;

    // Texture

    void glCreateTextures(GLenum target, GLsizei n, GLuint* textures) const;
    void glDeleteTextures(GLsizei n, GLuint* textures) const;
    void glBindTexture(GLenum target, GLuint texture) const;
    void glTextureParameteri(GLuint texture, GLenum pname, GLint param) const;
    void glGetTextureParameteriv(GLuint texture, GLenum pname, GLint* param) const;
    void glGetTextureLevelParameteriv(GLuint texture,
                                      GLint level,
                                      GLenum pname,
                                      GLint* param) const;
    void glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat* param) const;
    void glTexImage3D(GLenum target,
                      GLint level,
                      GLint internalformat,
                      GLsizei width,
                      GLsizei height,
                      GLsizei depth,
                      GLint border,
                      GLenum format,
                      GLenum type,
                      const void* pixels) const;
    void glTextureStorage3D(GLuint texture,
                            GLsizei levels,
                            GLenum internalformat,
                            GLsizei width,
                            GLsizei height,
                            GLsizei depth) const;
    void glTextureSubImage3D(GLuint texture,
                             GLint level,
                             GLint xoffset,
                             GLint yoffset,
                             GLint zoffset,
                             GLsizei width,
                             GLsizei height,
                             GLsizei depth,
                             GLenum format,
                             GLenum type,
                             const void* pixels) const;
    void glGenerateTextureMipmap(GLuint texture) const;
    void glCopyImageSubData(GLuint srcName,
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
                            GLsizei srcDepth) const;
    void glGetTextureSubImage(GLuint texture,
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
                              void* pixels) const;

    // Sampler

    void glCreateSamplers(GLsizei n, GLuint* samplers) const;
    void glDeleteSamplers(GLsizei n, GLuint* samplers) const;
    void glBindSampler(GLuint unit, GLuint sampler) const;
    void glSamplerParameteri(GLuint sampler, GLuint pname, GLint param) const;
    void glSamplerParameterf(GLuint sampler, GLuint pname, GLfloat param) const;
    void glSamplerParameterfv(GLuint sampler, GLuint pname, const GLfloat* param) const;

    // VertexArray

    void glCreateVertexArrays(GLsizei n, GLuint* arrays) const;
    void glDeleteVertexArrays(GLsizei n, GLuint* arrays) const;
    void glBindVertexArray(GLuint vaobj) const;
    void glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer) const;
    void glVertexArrayVertexBuffer(GLuint vaobj,
                                   GLuint bindingindex,
                                   GLuint buffer,
                                   GLintptr offset,
                                   GLsizei stride) const;
    void glVertexArrayAttribBinding(GLuint vaobj,
                                    GLuint attribindex,
                                    GLuint bindingindex) const;
    void glVertexArrayAttribFormat(GLuint vaobj,
                                   GLuint attribindex,
                                   GLint size,
                                   GLenum type,
                                   GLboolean normalized,
                                   GLuint relativeoffset) const;
    void glVertexArrayBindingDivisor(GLuint vaobj,
                                     GLuint bindingindex,
                                     GLuint divisor) const;
    void glEnableVertexArrayAttrib(GLuint vaobj, GLuint index) const;
    void glDisableVertexArrayAttrib(GLuint vaobj, GLuint index) const;

private:
#ifdef WIN32
    static std::unique_ptr<unsigned long, void (*)(unsigned long*)> tls_index;
#else
    static thread_local std::unique_ptr<Ctx> thread_ctx;
#endif

    Ctx(LoadFunc load_func);

    std::unique_ptr<GladGLContext> glad;
    Logger logger;

    // CtxLogger getLogger(const SrcLoc& src_loc = SrcLoc::Current()) const;
};

} // namespace gnev::gl
