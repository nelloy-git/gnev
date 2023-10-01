#include "gl/Buffer.hpp"

#include <iostream>

using namespace gnev::gl;

Buffer::Buffer(const Ctx& ctx)
    : Handler(ctx, createHandle(ctx), &freeHandle) {}

Buffer::~Buffer() {}

void Buffer::bind(GLenum target) const { ctx().glBindBuffer(target, handle()); }

void Buffer::bindBase(GLenum target, GLuint index) const {
    ctx().glBindBufferBase(target, index, handle());
}

void Buffer::bindRange(GLenum target,
                       GLuint index,
                       GLintptr offset,
                       GLsizeiptr size) const {
    ctx().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::initData(GLsizeiptr size, const void* data, GLenum usage) {
    ctx().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::initStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    ctx().glNamedBufferStorage(handle(), size, data, flags);
}

void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    ctx().glNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::getSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    ctx().glGetNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::copyTo(Buffer& writeBuffer,
                    GLintptr readOffset,
                    GLintptr writeOffset,
                    GLsizeiptr size) const {
    ctx().glCopyNamedBufferSubData(handle(),
                                   writeBuffer.handle(),
                                   readOffset,
                                   writeOffset,
                                   size);
}

void Buffer::getParameteriv(GLenum pname, GLint* params) const {
    ctx().glGetNamedBufferParameteriv(handle(), pname, params);
}

void* Buffer::map(GLenum access) { return ctx().glMapNamedBuffer(handle(), access); }

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    return ctx().glMapNamedBufferRange(handle(), offset, length, access);
}

void Buffer::flushRange(GLintptr offset, GLsizeiptr length) {
    ctx().glFlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::unmap() { ctx().glUnmapNamedBuffer(handle()); }

GLuint Buffer::createHandle(const Ctx& ctx) {
    GLuint handle;
    ctx.glCreateBuffers(1, &handle);
    return handle;
}

void Buffer::freeHandle(const Ctx& ctx, GLuint handle) {
    ctx.glDeleteBuffers(1, &handle);
}