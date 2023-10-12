#include "gl/Buffer.hpp"

using namespace gnev::gl;

Buffer::Buffer()
    : Handler(createHandle(), &deleteHandle) {}

Buffer::~Buffer() {}

void Buffer::bind(GLenum target) const { Ctx::Get().glBindBuffer(target, handle()); }

void Buffer::bindBase(GLenum target, GLuint index) const {
    Ctx::Get().glBindBufferBase(target, index, handle());
}

void Buffer::bindRange(GLenum target,
                       GLuint index,
                       GLintptr offset,
                       GLsizeiptr size) const {
    Ctx::Get().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::initData(GLsizeiptr size, const void* data, GLenum usage) {
    Ctx::Get().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::initStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    Ctx::Get().glNamedBufferStorage(handle(), size, data, flags);
}

void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    Ctx::Get().glNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::getSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    Ctx::Get().glGetNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::copyTo(Buffer& writeBuffer,
                    GLintptr readOffset,
                    GLintptr writeOffset,
                    GLsizeiptr size) const {
    Ctx::Get().glCopyNamedBufferSubData(handle(),
                                        writeBuffer.handle(),
                                        readOffset,
                                        writeOffset,
                                        size);
}

void Buffer::getParameteriv(GLenum pname, GLint* params) const {
    Ctx::Get().glGetNamedBufferParameteriv(handle(), pname, params);
}

void* Buffer::map(GLenum access) { return Ctx::Get().glMapNamedBuffer(handle(), access); }

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    return Ctx::Get().glMapNamedBufferRange(handle(), offset, length, access);
}

void Buffer::flushRange(GLintptr offset, GLsizeiptr length) {
    Ctx::Get().glFlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::unmap() { Ctx::Get().glUnmapNamedBuffer(handle()); }

GLuint* Buffer::createHandle() {
    auto handle = new GLuint(0);
    Ctx::Get().glCreateBuffers(1, handle);
    return handle;
}

void Buffer::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteBuffers(1, handle);
    delete handle;
}