#include "gl/Buffer.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "gl/fmt/Enum.hpp"
#include "gl/fmt/HandlerLog.hpp"
#include "util/Log.hpp"

namespace gnev::gl {

Buffer::Buffer()
    : Handler(createHandle(), &deleteHandle) {

    L2()->log();
}

Buffer::~Buffer() { L2()->log(); }

void Buffer::bind(GLenum target) const {
    L2()->log(fmt::Enum{target});
    Ctx::Get().glBindBuffer(target, handle());
}

void Buffer::bindBase(GLenum target, GLuint index) const {
    L2()->log(fmt::Enum{target}, index);
    Ctx::Get().glBindBufferBase(target, index, handle());
}

void Buffer::bindRange(GLenum target,
                       GLuint index,
                       GLintptr offset,
                       GLsizeiptr size) const {
    L2()->log(fmt::Enum{target}, index, offset, size);
    Ctx::Get().glBindBufferRange(target, index, handle(), offset, size);
}

void Buffer::initData(GLsizeiptr size, const void* data, GLenum usage) {
    L2()->log(size, data, fmt::Enum{usage});
    Ctx::Get().glNamedBufferData(handle(), size, data, usage);
}

void Buffer::initStorage(GLsizeiptr size, const void* data, GLbitfield flags) {
    L2()->log(size, data, fmt::BitFlags{flags, fmt::BitFlags::Group::glBufferStorage});
    Ctx::Get().glNamedBufferStorage(handle(), size, data, flags);
}

void Buffer::setSubData(GLintptr offset, GLsizeiptr size, const void* data) {
    L2()->log(offset, size, data);
    Ctx::Get().glNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::getSubData(GLintptr offset, GLsizeiptr size, void* data) const {
    L2()->log(offset, size, data);
    Ctx::Get().glGetNamedBufferSubData(handle(), offset, size, data);
}

void Buffer::copyTo(Buffer& writeBuffer,
                    GLintptr readOffset,
                    GLintptr writeOffset,
                    GLsizeiptr size) const {
    L2()->log(writeBuffer.handle(), readOffset, writeOffset, size);
    Ctx::Get().glCopyNamedBufferSubData(handle(),
                                        writeBuffer.handle(),
                                        readOffset,
                                        writeOffset,
                                        size);
}

GLint Buffer::getSize() const {
    GLint size;
    Ctx::Get().glGetNamedBufferParameteriv(handle(), GL_BUFFER_SIZE, &size);
    L2()->logRes(size);
    return size;
}

bool Buffer::isStorage() const {
    GLint is_storage;
    Ctx::Get().glGetNamedBufferParameteriv(handle(),
                                           GL_BUFFER_IMMUTABLE_STORAGE,
                                           &is_storage);
    L2()->logRes(is_storage == GL_TRUE);
    return is_storage == GL_TRUE;
}

GLbitfield Buffer::getStorageFlags() const {
    GLbitfield flags;
    Ctx::Get().glGetNamedBufferParameteriv(handle(),
                                           GL_BUFFER_STORAGE_FLAGS,
                                           reinterpret_cast<GLint*>(&flags));
    L2()->logRes(fmt::BitFlags{flags, fmt::BitFlags::Group::glBufferStorage});
    return flags;
}

void* Buffer::map(GLenum access) {
    L2()->log(fmt::Enum{access});
    return Ctx::Get().glMapNamedBuffer(handle(), access);
}

void* Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access) {
    L2()->log(offset, length, fmt::Enum{access});
    return Ctx::Get().glMapNamedBufferRange(handle(), offset, length, access);
}

void Buffer::flushRange(GLintptr offset, GLsizeiptr length) {
    L2()->log(offset, length);
    Ctx::Get().glFlushMappedNamedBufferRange(handle(), offset, length);
}

void Buffer::unmap() {
    L2()->log();
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