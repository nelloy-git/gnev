#include "gl/buffer/Accessor.hpp"

#include <cstring>

namespace gnev::gl::buffer {

Accessor::Accessor(Ref<gl::Buffer> buffer)
    : buffer(buffer) {}

AccessorSubData::AccessorSubData(Ref<gl::Buffer> buffer)
    : Accessor(buffer) {

    if (buffer->isStorage()) {
        GLbitfield storage_flags = buffer->getStorageFlags();
        if (not(storage_flags & GL_DYNAMIC_STORAGE_BIT)) {
            throw std::logic_error("");
        }
    }
}

void AccessorSubData::set(GLintptr offset, GLintptr size, const void* src) {
    buffer->setSubData(offset, size, src);
}

void AccessorSubData::get(GLintptr offset, GLintptr size, void* dst) {
    buffer->getSubData(offset, size, dst);
}

void AccessorSubData::change(GLintptr offset, GLintptr size, const Changer& changer) {
    void* data = std::malloc(size);
    get(offset, size, data);
    changer(data, size);
    set(offset, size, data);
}

AccessorCoherent::AccessorCoherent(Ref<gl::Buffer> buffer)
    : Accessor(buffer) {
    if (not buffer->isStorage()) {
        throw std::logic_error("");
    }

    GLbitfield storage_flags = buffer->getStorageFlags();
    if (not(storage_flags & GL_MAP_COHERENT_BIT) and
        not(storage_flags & GL_CLIENT_STORAGE_BIT)) {
        throw std::logic_error("");
    }

    map = static_cast<GLbyte*>(buffer->mapRange(0,
                                                buffer->getSize(),
                                                GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
                                                    GL_MAP_PERSISTENT_BIT |
                                                    GL_MAP_COHERENT_BIT));
}

AccessorCoherent::~AccessorCoherent() { buffer->unmap(); }

void AccessorCoherent::set(GLintptr offset, GLintptr size, const void* src) {
    std::memcpy(map + offset, src, size);
}

void AccessorCoherent::get(GLintptr offset, GLintptr size, void* dst) {
    std::memcpy(dst, map + offset, size);
}

void AccessorCoherent::change(GLintptr offset, GLintptr size, const Changer& changer) {
    changer(map + offset, size);
}

} // namespace gnev::gl::buffer