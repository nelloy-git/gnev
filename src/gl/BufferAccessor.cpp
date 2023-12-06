#include "gl/BufferAccessor.hpp"

#include <cstring>

namespace gnev::gl {

BufferAccessor::BufferAccessor(Ref<gl::Buffer> buffer)
    : buffer(buffer) {}

GLuint BufferAccessor::getBufferSize() const {
    int size = 0;
    buffer->getParameteriv(GL_BUFFER_SIZE, &size);
    return size;
}

BufferAccessorSubData::BufferAccessorSubData(Ref<gl::Buffer> buffer)
    : BufferAccessor(buffer) {
    int is_immutable;
    buffer->getParameteriv(GL_BUFFER_IMMUTABLE_STORAGE, &is_immutable);

    if (is_immutable) {
        int storage_flags;
        buffer->getParameteriv(GL_BUFFER_STORAGE_FLAGS, &storage_flags);
        if (not(storage_flags & GL_DYNAMIC_STORAGE_BIT)) {
            throw std::logic_error("");
        }
    }
}

void BufferAccessorSubData::set(GLintptr offset, GLintptr size, const void* src) {
    buffer->setSubData(offset, size, src);
}

void BufferAccessorSubData::get(GLintptr offset, GLintptr size, void* dst) {
    buffer->getSubData(offset, size, dst);
}

void BufferAccessorSubData::change(GLintptr offset,
                                   GLintptr size,
                                   const Changer& changer) {
    void* data = std::malloc(size);
    get(offset, size, data);
    changer(buffer, data, size);
    set(offset, size, data);
}

BufferAccessorCoherent::BufferAccessorCoherent(Ref<gl::Buffer> buffer)
    : BufferAccessor(buffer) {
    int is_immutable;
    buffer->getParameteriv(GL_BUFFER_IMMUTABLE_STORAGE, &is_immutable);
    if (is_immutable == GL_FALSE) {
        throw std::logic_error("");
    }

    int storage_flags;
    buffer->getParameteriv(GL_BUFFER_STORAGE_FLAGS, &storage_flags);
    if (not(storage_flags & GL_MAP_COHERENT_BIT) and
        not(storage_flags & GL_CLIENT_STORAGE_BIT)) {
        throw std::logic_error("");
    }

    map = static_cast<GLbyte*>(buffer->mapRange(0,
                                                getBufferSize(),
                                                GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
                                                    GL_MAP_PERSISTENT_BIT |
                                                    GL_MAP_COHERENT_BIT));
}

BufferAccessorCoherent::~BufferAccessorCoherent() { buffer->unmap(); }

void BufferAccessorCoherent::set(GLintptr offset, GLintptr size, const void* src) {
    std::memcpy(map + offset, src, size);
}

void BufferAccessorCoherent::get(GLintptr offset, GLintptr size, void* dst) {
    std::memcpy(dst, map + offset, size);
}

void BufferAccessorCoherent::change(GLintptr offset,
                                    GLintptr size,
                                    const Changer& changer) {
    changer(buffer, map + offset, size);
}

} // namespace gnev::gl