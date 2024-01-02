#include "gl/Buffer.hpp"

#include "gl/EnumFmt.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

Buffer::Buffer()
    : Handler(createHandle(), &deleteHandle) {
    GNEV_TRACE_L2("Buffer_{}::ctor", handle());
}

Buffer::~Buffer() { GNEV_TRACE_L2("Buffer_{}::destr", handle()); }

void Buffer::bind(GLenum target) const {
    GNEV_TRACE_L2("Buffer_{}::bind({})", handle(), EnumFmt{target});
    Ctx::Get().glBindBuffer(target, handle());
}

void Buffer::bindBase(GLenum target, GLuint index) const {
    GNEV_TRACE_L2("Buffer_{}::bindBase({}, {})", handle(), EnumFmt{target}, index);
    Ctx::Get().glBindBufferBase(target, index, handle());
}

void Buffer::bindRange(GLenum target,
                       GLuint index,
                       GLintptr offset,
                       GLsizeiptr size) const {
    GNEV_TRACE_L2("Buffer_{}::bindRange({}, {}, {}, {})",
                  handle(),
                  EnumFmt{target},
                  index,
                  offset,
                  size);
    Ctx::Get().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::initData(GLsizeiptr size, const void* data, GLenum usage) {
    GNEV_TRACE_L2("Buffer_{}::initData({}, {})", handle(), size, data, EnumFmt{usage});
    Ctx::Get().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::initStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    GNEV_TRACE_L2("Buffer_{}::initStorage({}, {}, {})",
                  handle(),
                  size,
                  data,
                  std::bitset<std::numeric_limits<GLbitfield>::digits>(flags));
    Ctx::Get().glNamedBufferStorage(handle(), size, data, flags);
}

void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    GNEV_TRACE_L2("Buffer_{}::setSubData({}, {}, {})", handle(), offset, size, data);
    Ctx::Get().glNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::getSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    GNEV_TRACE_L2("Buffer_{}::getSubData({}, {}, {})", handle(), offset, size, data);
    Ctx::Get().glGetNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::copyTo(Buffer& writeBuffer,
                    GLintptr readOffset,
                    GLintptr writeOffset,
                    GLsizeiptr size) const {
    GNEV_TRACE_L2("Buffer_{}::copyTo(Buffer_{}, {}, {}, {})",
                  handle(),
                  writeBuffer.handle(),
                  readOffset,
                  writeOffset,
                  size);
    Ctx::Get().glCopyNamedBufferSubData(handle(),
                                        writeBuffer.handle(),
                                        readOffset,
                                        writeOffset,
                                        size);
}

void Buffer::getParameteriv(GLenum pname, GLint* params) const {
    Ctx::Get().glGetNamedBufferParameteriv(handle(), pname, params);
    GNEV_TRACE_L2("Buffer_{}::getParameteriv({}, {}) -> {}...",
                  handle(),
                  EnumFmt{pname},
                  static_cast<void*>(params),
                  params[0]);
}

void* Buffer::map(GLenum access) {
    GNEV_TRACE_L2("Buffer_{}::map({})", handle(), EnumFmt{access});
    return Ctx::Get().glMapNamedBuffer(handle(), access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    GNEV_TRACE_L2("Buffer_{}::mapRange({}, {}, {})",
                  handle(),
                  offset,
                  length,
                  EnumFmt{access});
    return Ctx::Get().glMapNamedBufferRange(handle(), offset, length, access);
}

void Buffer::flushRange(GLintptr offset, GLsizeiptr length) {
    GNEV_TRACE_L2("Buffer_{}::flushRange({}, {})", handle(), offset, length);
    Ctx::Get().glFlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::unmap() {
    GNEV_TRACE_L2("Buffer_{}::unmap()", handle());
    Ctx::Get().glUnmapNamedBuffer(handle());
}

GLuint* Buffer::createHandle() {
    auto handle = new GLuint(0);
    Ctx::Get().glCreateBuffers(1, handle);
    return handle;
}

void Buffer::deleteHandle(GLuint* handle) {
    Ctx::Get().glDeleteBuffers(1, handle);
    delete handle;
}