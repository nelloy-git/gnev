#include "gl/Ctx.hpp"

using namespace gnev::gl;

Ctx::Ctx(LoadFunc load_func)
    : glad(std::make_shared<GladGLContext>()) {
    gladLoadGLContext(glad.get(), load_func);
}

Ctx::~Ctx() {}

void Ctx::glGetIntegerv(GLenum pname, GLint* params) const {
    glad->GetIntegerv(pname, params);
}

void Ctx::glEnable(GLenum pname) const { glad->Enable(pname); }

void Ctx::glDebugMessageCallback(GLDEBUGPROC callback, const void* userParam) const {
    glad->DebugMessageCallback(callback, userParam);
}

void Ctx::glDebugMessageControl(GLenum source,
                                GLenum type,
                                GLenum severity,
                                GLsizei count,
                                const GLuint* ids,
                                GLboolean enabled) const {
    glad->DebugMessageControl(source, type, severity, count, ids, enabled);
}

void Ctx::glCreateBuffers(GLsizei n, GLuint* buffers) const {
    glad->CreateBuffers(n, buffers);
}

void Ctx::glDeleteBuffers(GLsizei n, GLuint* buffers) const {
    glad->DeleteBuffers(n, buffers);
}

void Ctx::glBindBuffer(GLenum target, GLuint buffer) const {
    glad->BindBuffer(target, buffer);
}

void Ctx::glBindBufferBase(GLenum target, GLuint index, GLuint buffer) const {
    glad->BindBufferBase(target, index, buffer);
}

void Ctx::glBindBufferRange(GLenum target,
                            GLuint index,
                            GLuint buffer,
                            GLintptr offset,
                            GLsizeiptr size) const {
    glad->BindBufferRange(target, index, buffer, offset, size);
}

void Ctx::glNamedBufferData(GLuint buffer,
                            GLsizeiptr size,
                            const void* data,
                            GLenum usage) const {
    glad->NamedBufferData(buffer, size, data, usage);
}

void Ctx::glNamedBufferStorage(GLuint buffer,
                               GLsizeiptr size,
                               const void* data,
                               GLbitfield flags) const {
    glad->NamedBufferStorage(buffer, size, data, flags);
}

void Ctx::glNamedBufferSubData(GLuint buffer,
                               GLintptr offset,
                               GLsizeiptr size,
                               const void* data) const {
    glad->NamedBufferSubData(buffer, offset, size, data);
}

void Ctx::glGetNamedBufferSubData(GLuint buffer,
                                  GLintptr offset,
                                  GLsizeiptr size,
                                  void* data) const {
    glad->GetNamedBufferSubData(buffer, offset, size, data);
}

void Ctx::glCopyNamedBufferSubData(GLuint readBuffer,
                                   GLuint writeBuffer,
                                   GLintptr readOffset,
                                   GLintptr writeOffset,
                                   GLsizeiptr size) const {
    glad->CopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset, size);
}

void Ctx::glGetNamedBufferParameteriv(GLuint buffer, GLenum pname, GLint* params) const {
    glad->GetNamedBufferParameteriv(buffer, pname, params);
}

void* Ctx::glMapNamedBuffer(GLuint buffer, GLenum access) const {
    return glad->MapNamedBuffer(buffer, access);
}

void* Ctx::glMapNamedBufferRange(GLuint buffer,
                                 GLintptr offset,
                                 GLsizeiptr length,
                                 GLbitfield access) const {
    return glad->MapNamedBufferRange(buffer, offset, length, access);
}

void Ctx::glFlushMappedNamedBufferRange(GLuint buffer,
                                        GLintptr offset,
                                        GLsizeiptr length) const {
    glad->FlushMappedNamedBufferRange(buffer, offset, length);
}

void Ctx::glUnmapNamedBuffer(GLuint buffer) const { glad->UnmapNamedBuffer(buffer); }

GLuint Ctx::glCreateShader(GLenum type) const { return glad->CreateShader(type); }

void Ctx::glDeleteShader(GLuint shader) const { glad->DeleteShader(shader); }

void Ctx::glShaderSource(GLuint shader,
                         GLsizei count,
                         const GLchar* const* string,
                         const GLint* length) const {
    glad->ShaderSource(shader, count, string, length);
}

void Ctx::glCompileShader(GLuint shader) const { glad->CompileShader(shader); }

void Ctx::glGetShaderiv(GLuint shader, GLenum pname, GLint* params) const {
    glad->GetShaderiv(shader, pname, params);
}

void Ctx::glGetShaderInfoLog(GLuint shader,
                             GLsizei bufSize,
                             GLsizei* length,
                             GLchar* infoLog) const {
    glad->GetShaderInfoLog(shader, bufSize, length, infoLog);
}

GLuint Ctx::glCreateProgram() const { return glad->CreateProgram(); }

void Ctx::glDeleteProgram(GLuint program) const { glad->DeleteProgram(program); }

void Ctx::glAttachShader(GLuint program, GLuint shader) const {
    glad->AttachShader(program, shader);
}

void Ctx::glValidateProgram(GLuint program) const { glad->ValidateProgram(program); }

void Ctx::glLinkProgram(GLuint program) const { glad->LinkProgram(program); }

void Ctx::glUseProgram(GLuint program) const { glad->UseProgram(program); }

void Ctx::glGetProgramiv(GLuint program, GLenum pname, GLint* params) const {
    glad->GetProgramiv(program, pname, params);
}

void Ctx::glGetProgramInfoLog(GLuint program,
                              GLsizei bufSize,
                              GLsizei* length,
                              GLchar* infoLog) const {
    glad->GetProgramInfoLog(program, bufSize, length, infoLog);
}

GLint Ctx::glGetUniformBlockIndex(GLuint program, const GLchar* uniformBlockName) const {
    return glad->GetUniformBlockIndex(program, uniformBlockName);
}

void Ctx::glUniformBlockBinding(GLuint program,
                                GLuint uniformBlockIndex,
                                GLuint uniformBlockBinding) const {
    glad->UniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding);
}

GLint Ctx::glGetProgramResourceIndex(GLuint program,
                                     GLenum programInterface,
                                     const GLchar* name) const {
    return glad->GetProgramResourceIndex(program, programInterface, name);
}

void Ctx::glShaderStorageBlockBinding(GLuint program,
                                      GLuint storageBlockIndex,
                                      GLuint storageBlockBinding) const {
    glad->ShaderStorageBlockBinding(program, storageBlockIndex, storageBlockBinding);
}

GLint Ctx::glGetAttribLocation(GLuint program, const GLchar* name) const {
    return glad->GetAttribLocation(program, name);
}

GLint Ctx::glGetUniformLocation(GLuint program, const GLchar* name) const {
    return glad->GetUniformLocation(program, name);
}

void Ctx::glCreateTextures(GLenum target, GLsizei n, GLuint* textures) const {
    glad->CreateTextures(target, n, textures);
}

void Ctx::glDeleteTextures(GLsizei n, GLuint* textures) const {
    glad->DeleteTextures(n, textures);
}

void Ctx::glBindTexture(GLenum target, GLuint texture) const {
    glad->BindTexture(target, texture);
}

void Ctx::glTextureParameteri(GLuint texture, GLenum pname, GLint param) const {
    glad->TextureParameteri(texture, pname, param);
}

void Ctx::glGetTextureParameteriv(GLuint texture, GLenum pname, GLint* param) const {
    glad->GetTextureParameteriv(texture, pname, param);
}

void Ctx::glGetTextureLevelParameteriv(GLuint texture,
                                       GLint level,
                                       GLenum pname,
                                       GLint* param) const {
    glad->GetTextureLevelParameteriv(texture, level, pname, param);
}

void Ctx::glTextureParameterfv(GLuint texture, GLenum pname, const GLfloat* param) const {
    glad->TextureParameterfv(texture, pname, param);
}

void Ctx::glTextureStorage3D(GLuint texture,
                             GLsizei levels,
                             GLenum internalformat,
                             GLsizei width,
                             GLsizei height,
                             GLsizei depth) const {
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
    glad->CreateSamplers(n, samplers);
}

void Ctx::glDeleteSamplers(GLsizei n, GLuint* samplers) const {
    glad->DeleteSamplers(n, samplers);
}

void Ctx::glBindSampler(GLuint unit, GLuint sampler) const {
    glad->BindSampler(unit, sampler);
}

void Ctx::glSamplerParameteri(GLuint sampler, GLuint pname, GLint param) const {
    glad->SamplerParameteri(sampler, pname, param);
}

void Ctx::glSamplerParameterf(GLuint sampler, GLuint pname, GLfloat param) const {
    glad->SamplerParameterf(sampler, pname, param);
}

void Ctx::glSamplerParameterfv(GLuint sampler, GLuint pname, const GLfloat* param) const {
    glad->SamplerParameterfv(sampler, pname, param);
}

void Ctx::glCreateVertexArrays(GLsizei n, GLuint* arrays) const {
    glad->CreateVertexArrays(n, arrays);
}

void Ctx::glDeleteVertexArrays(GLsizei n, GLuint* arrays) const {
    glad->DeleteVertexArrays(n, arrays);
}

void Ctx::glBindVertexArray(GLuint vaobj) const { glad->BindVertexArray(vaobj); }

void Ctx::glVertexArrayElementBuffer(GLuint vaobj, GLuint buffer) const {
    glad->VertexArrayElementBuffer(vaobj, buffer);
}

void Ctx::glVertexArrayVertexBuffer(GLuint vaobj,
                                    GLuint bindingindex,
                                    GLuint buffer,
                                    GLintptr offset,
                                    GLsizei stride) const {
    glad->VertexArrayVertexBuffer(vaobj, bindingindex, buffer, offset, stride);
}

void Ctx::glVertexArrayAttribBinding(GLuint vaobj,
                                     GLuint attribindex,
                                     GLuint bindingindex) const {
    glad->VertexArrayAttribBinding(vaobj, attribindex, bindingindex);
}

void Ctx::glVertexArrayAttribFormat(GLuint vaobj,
                                    GLuint attribindex,
                                    GLint size,
                                    GLenum type,
                                    GLboolean normalized,
                                    GLuint relativeoffset) const {
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
    glad->VertexArrayBindingDivisor(vaobj, bindingindex, divisor);
}

void Ctx::glEnableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    glad->EnableVertexArrayAttrib(vaobj, index);
}

void Ctx::glDisableVertexArrayAttrib(GLuint vaobj, GLuint index) const {
    glad->DisableVertexArrayAttrib(vaobj, index);
}