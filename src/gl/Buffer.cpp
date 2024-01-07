#include "gl/Buffer.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "util/Log.hpp"

using namespace gnev::gl;

Buffer::Buffer()
    : Handler(createHandle(), &deleteHandle) {
    GNEV_TRACE_L2("Buffer_{}::ctor()", handle());
}

Buffer::~Buffer() { GNEV_TRACE_L2("Buffer_{}::destr()", handle()); }

void Buffer::bind(GLenum target) const {
    GNEV_TRACE_L2("Buffer_{}::bind({})", handle(), fmt::Enum{target});
    Ctx::Get().glBindBuffer(target, handle());
}

void Buffer::bindBase(GLenum target, GLuint index) const {
    GNEV_TRACE_L2("Buffer_{}::bindBase({}, {})", handle(), fmt::Enum{target}, index);
    Ctx::Get().glBindBufferBase(target, index, handle());
}

void Buffer::bindRange(GLenum target,
                       GLuint index,
                       GLintptr offset,
                       GLsizeiptr size) const {
    GNEV_TRACE_L2("Buffer_{}::bindRange({}, {}, {}, {})",
                  handle(),
                  fmt::Enum{target},
                  index,
                  offset,
                  size);
    Ctx::Get().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::initData(GLsizeiptr size, const void* data, GLenum usage) {
    GNEV_TRACE_L2("Buffer_{}::initData({}, {})", handle(), size, data, fmt::Enum{usage});
    Ctx::Get().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::initStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    GNEV_TRACE_L2("Buffer_{}::initStorage({}, {}, {})",
                  handle(),
                  size,
                  data,
                  fmt::BitFlags{flags, fmt::BitFlags::Group::glBufferStorage});
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

GLint Buffer::getSize() const {
    GLint size;
    Ctx::Get().glGetNamedBufferParameteriv(handle(), GL_BUFFER_SIZE, &size);
    GNEV_TRACE_L2("Buffer_{}::getSize() -> {}", handle(), size);
    return size;
}

bool Buffer::isStorage() const {
    GLint is_storage;
    Ctx::Get().glGetNamedBufferParameteriv(handle(),
                                           GL_BUFFER_IMMUTABLE_STORAGE,
                                           &is_storage);
    GNEV_TRACE_L2("Buffer_{}::isStorage() -> {}", handle(), is_storage == GL_TRUE);
    return is_storage == GL_TRUE;
}

GLbitfield Buffer::getStorageFlags() const {
    GLbitfield flags;
    Ctx::Get().glGetNamedBufferParameteriv(handle(),
                                           GL_BUFFER_STORAGE_FLAGS,
                                           reinterpret_cast<GLint*>(&flags));
    GNEV_TRACE_L2("Buffer_{}::getStorageFlags() -> {}",
                  handle(),
                  fmt::BitFlags{flags, fmt::BitFlags::Group::glBufferStorage});
    return flags;
}

void* Buffer::map(GLenum access) {
    GNEV_TRACE_L2("Buffer_{}::map({})", handle(), fmt::Enum{access});
    return Ctx::Get().glMapNamedBuffer(handle(), access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    GNEV_TRACE_L2("Buffer_{}::mapRange({}, {}, {})",
                  handle(),
                  offset,
                  length,
                  fmt::Enum{access});
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