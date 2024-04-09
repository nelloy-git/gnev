#include "gl/Buffer.hpp"

#include "gl/enum/BufferStorageFlags.hpp"
#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/logger/HandlerLogger.hpp"

namespace gnev::gl {

Buffer::Buffer()
    : Handler(createHandle(), &deleteHandle) {
    Log().Func();
}

Buffer::~Buffer() { Log().Func(); }

void Buffer::bind(GLenum target) const {
    Log().Func(fmt::Enum{target});
    Ctx::Get().glBindBuffer(target, handle());
}

void Buffer::bindBase(GLenum target, GLuint index) const {
    Log().Func(fmt::Enum{target}, index);
    Ctx::Get().glBindBufferBase(target, index, handle());
}

void Buffer::bindRange(GLenum target,
                       GLuint index,
                       GLintptr offset,
                       GLsizeiptr size) const {
    Log().Func(fmt::Enum{target}, index, offset, size);
    Ctx::Get().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::initData(GLsizeiptr size, const void* data, GLenum usage) {
    Log().Func(size, data, fmt::Enum{usage});
    Ctx::Get().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::initStorage(GLsizeiptr size, const void* data, BufferStorageFlags flags) {
    Log().Func(size, data, flags);
    Ctx::Get().glNamedBufferStorage(handle(), size, data, static_cast<GLenum>(flags));
}

void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    Log().Func(offset, size, data);
    Ctx::Get().glNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::getSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    Log().Func(offset, size, data);
    Ctx::Get().glGetNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::copyTo(Buffer& writeBuffer,
                    GLintptr readOffset,
                    GLintptr writeOffset,
                    GLsizeiptr size) const {
    Log().Func(writeBuffer.handle(), readOffset, writeOffset, size);
    Ctx::Get().glCopyNamedBufferSubData(handle(),
                                        writeBuffer.handle(),
                                        readOffset,
                                        writeOffset,
                                        size);
}

GLint Buffer::getSize() const {
    Log().Func();
    GLint size;
    Ctx::Get().glGetNamedBufferParameteriv(handle(), GL_BUFFER_SIZE, &size);
    return size;
}

bool Buffer::isStorage() const {
    Log().Func();
    GLint is_storage;
    Ctx::Get().glGetNamedBufferParameteriv(handle(),
                                           GL_BUFFER_IMMUTABLE_STORAGE,
                                           &is_storage);
    return is_storage == GL_TRUE;
}

BufferStorageFlags Buffer::getStorageFlags() const {
    Log().Func();
    BufferStorageFlags flags;
    Ctx::Get().glGetNamedBufferParameteriv(handle(),
                                           GL_BUFFER_STORAGE_FLAGS,
                                           reinterpret_cast<GLint*>(&flags));
    return flags;
}

void* Buffer::map(GLenum access) {
    Log().Func(fmt::Enum{access});
    return Ctx::Get().glMapNamedBuffer(handle(), access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, BufferMapRangeAccess access) {
    Log().Func(offset, length, access);
    return Ctx::Get().glMapNamedBufferRange(handle(),
                                            offset,
                                            length,
                                            static_cast<GLenum>(access));
}

void Buffer::flushRange(GLintptr offset, GLsizeiptr length) {
    Log().Func(offset, length);
    Ctx::Get().glFlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::unmap() {
    Log().Func();
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

} // namespace gnev::gl