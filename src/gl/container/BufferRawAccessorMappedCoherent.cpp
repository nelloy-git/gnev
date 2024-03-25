#include "gl/container/BufferRawAccessorMappedCoherent.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

void BufferRawAccessorMappedCoherent::bindBuffer(Buffer* new_buffer) {
    if (buffer) {
        buffer->unmap();
    }

    buffer = new_buffer;
    map = nullptr;

    if (not buffer) {
        return;
    }

    if (not buffer->isStorage()) {
        InstanceLogger{}.Log<ERROR, "Buffer<{}> is not storage">(buffer->handle());
    }

    GLbitfield storage_flags = buffer->getStorageFlags();
    if (not(storage_flags & GL_MAP_COHERENT_BIT) and
        not(storage_flags & GL_CLIENT_STORAGE_BIT)) {
        InstanceLogger{}
            .Log<ERROR,
                 "Buffer<{}> has invalid storage flags {}">(buffer->handle(),
                                                            fmt::BitFlags{
                                                                storage_flags,
                                                                fmt::BitFlags::Group::
                                                                    glBufferStorage});
    }

    map = static_cast<GLbyte*>(buffer->mapRange(0,
                                                buffer->getSize(),
                                                GL_MAP_READ_BIT | GL_MAP_WRITE_BIT |
                                                    GL_MAP_PERSISTENT_BIT |
                                                    GL_MAP_COHERENT_BIT));
}

Buffer* BufferRawAccessorMappedCoherent::getBoundBuffer() const {
    return buffer;
}

BufferRawAccessorMappedCoherent::~BufferRawAccessorMappedCoherent() {
    if (buffer) {
        buffer->unmap();
    }
}

bool BufferRawAccessorMappedCoherent::set(unsigned offset,
                                          unsigned size,
                                          const void* src) {
    if (not map) {
        return false;
    }
    std::memcpy(map + offset, src, size);
    return true;
}

bool BufferRawAccessorMappedCoherent::get(unsigned offset, unsigned size, void* dst) {
    if (not map) {
        return false;
    }
    std::memcpy(dst, map + offset, size);
    return true;
}

bool BufferRawAccessorMappedCoherent::change(unsigned offset,
                                             unsigned size,
                                             const Changer& changer) {
    if (not map) {
        return false;
    }
    changer(map + offset, size);
    return true;
}

bool BufferRawAccessorMappedCoherent::copy(unsigned src_offset,
                                           unsigned dst_offset,
                                           unsigned size) {
    if (not map) {
        return false;
    }
    std::memcpy(map + dst_offset, map + src_offset, size);
    return true;
}

} // namespace gnev::gl