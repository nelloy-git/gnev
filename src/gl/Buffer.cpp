#include "gl/Buffer.hpp"

#include <iostream>

using namespace gnev::gl;

Buffer::Buffer(const Ctx& ctx)
    : Handler(ctx, createHandle(ctx), &freeHandle) {}

Buffer::~Buffer() {}

void Buffer::glBindBuffer(GLenum target) const { ctx().glBindBuffer(target, handle()); }

void Buffer::glBindBufferBase(GLenum target, GLuint index) const {
    ctx().glBindBufferBase(target, index, handle());
}

void Buffer::glBindBufferRange(GLenum target,
                               GLuint index,
                               GLintptr offset,
                               GLsizeiptr size) const {
    ctx().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::glBufferData(GLsizeiptr size, const void* data, GLenum usage) {
    ctx().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::glBufferStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    ctx().glNamedBufferStorage(handle(), size, data, flags);
}

void Buffer::glBufferSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    ctx().glNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::glGetBufferSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    ctx().glGetNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::glCopyBufferSubData(GLuint writeBuffer,
                                 GLintptr readOffset,
                                 GLintptr writeOffset,
                                 GLsizeiptr size) const {
    ctx().glCopyNamedBufferSubData(handle(), writeBuffer, readOffset, writeOffset, size);
}

void Buffer::glGetBufferParameteriv(GLenum pname, GLint* params) const {
    ctx().glGetNamedBufferParameteriv(handle(), pname, params);
}

void* Buffer::glMapBuffer(GLenum access) {
    return ctx().glMapNamedBuffer(handle(), access);
}

void* Buffer::glMapBufferRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    return ctx().glMapNamedBufferRange(handle(), offset, length, access);
}

void Buffer::glFlushMappedBufferRange(GLintptr offset, GLsizeiptr length) {
    ctx().glFlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::glUnmapBuffer() { ctx().glUnmapNamedBuffer(handle()); }

GLuint Buffer::createHandle(const Ctx& ctx) {
    GLuint handle;
    ctx.glCreateBuffers(1, &handle);
    return handle;
}

void Buffer::freeHandle(const Ctx& ctx, GLuint handle) { ctx.glDeleteBuffers(1, &handle); }