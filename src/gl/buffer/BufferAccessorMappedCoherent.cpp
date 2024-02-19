#include "gl/buffer/BufferAccessorMappedCoherent.hpp"

#include "gl/fmt/BitFlags.hpp"
#include "util/InstanceLogger.hpp"

namespace gnev::gl {

BufferAccessorMappedCoherent::BufferAccessorMappedCoherent(std::unique_ptr<Buffer>&& buffer)
    : buffer{std::move(buffer)} {

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

const Buffer& BufferAccessorMappedCoherent::getBuffer() const { return *buffer; }

BufferAccessorMappedCoherent::~BufferAccessorMappedCoherent() { buffer->unmap(); }

void BufferAccessorMappedCoherent::set(GLintptr offset, GLintptr size, const void* src) {
    std::memcpy(map + offset, src, size);
}

void BufferAccessorMappedCoherent::get(GLintptr offset, GLintptr size, void* dst) {
    std::memcpy(dst, map + offset, size);
}

void BufferAccessorMappedCoherent::change(GLintptr offset,
                                          GLintptr size,
                                          const Changer& changer) {
    changer(map + offset, size);
}

} // namespace gnev::gl